#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

std::pair<std::vector<int>, std::vector<int>> generate_broadcast_shape(
          const std::vector<int>& shape, const std::vector<int>& second_shape) {

    // 1. Expand the source shape with insertions of dimensions of 1
    std::vector<int> augmented_shape = shape;
    while (augmented_shape.size() < second_shape.size()) {
        augmented_shape.insert(augmented_shape.begin(), 1);
    }
    // 2. Compute the broadcast shape
    std::vector<int> broadcast_shape = augmented_shape;
    std::vector<int> incompatible_dimensions;
    for (int i = 0; i < augmented_shape.size(); i++) {
        if (augmented_shape[i] == 1) {
            broadcast_shape[i] = second_shape[i];
        }
        else if (second_shape[i] == 1) {
            broadcast_shape[i] = augmented_shape[i];
        }
        else if (augmented_shape[i] == second_shape[i]) {
            broadcast_shape[i] = augmented_shape[i];
        }
        else {
            incompatible_dimensions.push_back(i);
        }
    }
    if(!incompatible_dimensions.empty()){
        std::string error_message = "Shapes are not compatible for broadcasting at dimension(s) ";
        for (auto d : incompatible_dimensions) {
            error_message += std::to_string(d) + " ";
        }
        throw std::runtime_error(error_message);
    }
    // Return the augmented shape and the broadcast shape
    return std::make_pair(augmented_shape, broadcast_shape);
}

template <typename T>
std::vector<T> nbroadcast(const std::vector<T>& input,
                         const std::vector<int> &shape,
                         const std::vector<int> &new_shape) {
    // Check if the input shape is compatible with the new shape
    if(shape.size() != new_shape.size()) {
        throw std::invalid_argument("Input shape and new shape must have the same number of dimensions.");
    }
    for(int i = 0; i < shape.size(); i++) {
        if(shape[i] != 1 && shape[i] != new_shape[i]) {
            throw std::invalid_argument("Input shape and new shape are incompatible.");
        }
    }
    std::vector<int> input_strides(shape.size());
    std::vector<int> new_strides(new_shape.size());
    input_strides[shape.size()-1] = 1;
    new_strides[new_shape.size()-1] = 1;
    for(int i= shape.size()-2; i>=0; i--)
        input_strides[i] = input_strides[i+1]*shape[i+1];
    for(int i= new_shape.size()-2; i>=0; i--)
        new_strides[i] = new_strides[i+1]*new_shape[i+1];
    std::vector<int> input_indices(shape.size(),0);
    std::vector<int> new_indices(new_shape.size(),0);
    std::vector<T> output(new_shape.size());
    int i = 0;
    while(i < new_shape.size()) {
        new_indices[i] = 0;
        i++;
    }
    while(i >= 0) {
        int input_idx = 0;
        int new_idx = 0;
        for(int k=0; k<shape.size(); k++)
            input_indices[k] = new_indices[k]%shape[k];
        for(int k=0; k<shape.size(); k++)
            input_idx += input_indices[k]*input_strides[k];
        for(int k=0; k<new_shape.size(); k++)
            new_idx += new_indices[k]*new_strides[k];
        output[new_idx] = input[input_idx];
        i--;
        while(i >= 0 && new_indices[i] == new_shape[i]-1) {
            new_indices[i] = 0;
            i--;
        }
        if(i >= 0) {
            new_indices[i]++;
            i = new_shape.size()-1;
        }
    }
    return output;
}

template <typename T>
void test_broadcast(const std::vector<T>& input,
                    std::vector<int> shape,
                    std::vector<int> new_shape,
                    const std::vector<T>& expected_output) {
    try {
        std::vector<T> output = broadcast(input, shape, new_shape);
        if(output != expected_output) {
            std::cout << "Test failed: output does not match expected output." << std::endl;
        } else {
            std::cout << "Test passed." << std::endl;
        }
    } catch(const std::exception& e) {
        std::cout << "Test failed: " << e.what() << std::endl;
    }
}


void test_broadcast_shape(const std::vector<int>& shape,
                          const std::vector<int>& second_shape,
                          const std::vector<int>& expected_augmented_shape,
                          const std::vector<int>& expected_broadcast_shape) {
    try {
        auto result = generate_broadcast_shape(shape, second_shape);
        if(std::get<0>(result) != expected_augmented_shape ||
           std::get<1>(result) != expected_broadcast_shape)
            throw std::runtime_error("Unexpected output, expected augmented shape : {" +
                  std::to_string(expected_augmented_shape[0]) +"," +
                  std::to_string(expected_augmented_shape[1])+"," +
                  std::to_string(expected_augmented_shape[2]) +
                  "} and broadcast shape : {" +
                  std::to_string(expected_broadcast_shape[0]) + "," +
                  std::to_string(expected_broadcast_shape[1])+"," +
                  std::to_string(expected_broadcast_shape[2])+"}" );
        std::cout << "Test passed, generated augmented shape : {" +
                  std::to_string(std::get<0>(result)[0]) + "," +
                  std::to_string(std::get<0>(result)[1]) + "," +
                  std::to_string(std::get<0>(result)[2]) +
                  "} and broadcast shape : {" +
                  std::to_string(std::get<1>(result)[0])+"," +
                  std::to_string(std::get<1>(result)[1])+"," +
                  std::to_string(std::get<1>(result)[2])+"}\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
  os << "[";
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    os << *it;
    if (std::next(it) != vec.end()) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

template <typename T>
std::vector<T> broadcast(const std::vector<T>& data,
               const std::vector<int>& data_shape,
               const std::vector<int>& shape) {

  std::cout << "data: " << data
      << ", data_shape: " << data_shape
      << ", shape: " << shape << std::endl;
  int total_size = 1;
  for (int i : shape) total_size *= i;
  std::vector<T> output(total_size);

  // calculate position of data to sample
  std::vector<int> pos(shape.size());
  std::vector<int> data_pos(data_shape.size());
  for (int i = 0; i < total_size; ++i) {
    int temp = i;
    for (int j = shape.size() - 1; j >= 0; --j) {
      int idx = temp % shape[j];
      // check broadcasting rules
      if (shape[j] > data_shape[j]) {
        // data_shape[j] should be 1
        data_pos[j] = 0;
      }
      else {
        data_pos[j] = idx;
      }
      pos[j] = idx;
      temp /= shape[j];
    }

    std::cout << "o[" << i << "] " <<  pos << " " << data_pos;

    int k = 0;
    int stride = 1;
    for (int i = data_shape.size() - 1; i >= 0; i--) {
      k += data_pos[i] * stride;
      stride *= data_shape[i];
    }

    output[i] = data[k];
    std::cout << " i[" << k  << "] = " << output[i] << std::endl;

  }
  return output;
}

int main() {
    /*
    test_broadcast_shape({2,3}, {1, 3}, {2, 3}, {2, 3});
    test_broadcast_shape({1, 3}, {2, 3, 3}, {1, 1, 3}, {2, 3, 3});
    test_broadcast_shape({1, 2, 3}, {2, 3, 3}, {1, 2, 3}, {2, 3, 3});
    std::cout << "Broadcast test" << std::endl;
    std::vector<int> input = {1,2,3};
    std::vector<int> output = {1,2,3,1,2,3};
    test_broadcast(input, {1,3}, {2,3}, output);
    */

    broadcast<double>({1.,2.,3.}, {3,1}, {3,2});
    broadcast<double>({1.,2.,3.}, {1,3}, {2,3});
    return 0;
}

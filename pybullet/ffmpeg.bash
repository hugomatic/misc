ffmpeg -r 30 -s 800x600 -i test_img/rgbtest%03d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p out.mp4

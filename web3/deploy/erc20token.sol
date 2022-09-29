// SPDX-License-Identifier: MIT
pragma solidity ^0.8.4;

import "./@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "./@openzeppelin/contracts/access/AccessControl.sol";

contract myTok is ERC20, AccessControl {
    bytes32 public constant MINTER_ROLE = keccak256("MINTER_ROLE");

    constructor() ERC20("myTok", "MTOK") {
        _mint(msg.sender, 10 * 10 ** decimals());
        _grantRole(DEFAULT_ADMIN_ROLE, msg.sender);
        _grantRole(MINTER_ROLE, msg.sender);
    }

    function mint(address to, uint256 amount) public onlyRole(MINTER_ROLE) {
        _mint(to, amount);
    }
}
// Framework
// Copyright (C) 2025 Ian Torres <iantorres@outlook.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#ifndef FRAMEWORK_ENCODING_BASE64URL_HPP
#define FRAMEWORK_ENCODING_BASE64URL_HPP

#include <array>
#include <string>
#include <string_view>

namespace framework::encoding {
  /**
   * Base64url charset
   */
  constexpr std::string_view BASE64URL_CHARSET =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

  /**
   * Base64url charset as map
   */
  inline constexpr std::array<int, 256> BASE64URL_CHARSET_MAP = []() {
    std::array<int, 256> _map{};
    _map.fill(-1);
    for (std::size_t i = 0; i < 64; ++i)
      _map[static_cast<unsigned char>(BASE64URL_CHARSET[i])] =
          static_cast<int>(i);
    return _map;
  }();

  /**
   * Encodes to Base64url
   *
   * @param input
   * @param padding
   * @return string
   */
  inline std::string base64url_encode(const std::string &input,
                                      const bool padding = true) noexcept {
    std::string _output;
    int _value = 0;
    int _value_b = -6;

    for (const unsigned char _character : input) {
      _value = (_value << 8) + _character;
      _value_b += 8;
      while (_value_b >= 0) {
        _output.push_back(BASE64URL_CHARSET[_value >> _value_b & 0x3F]);
        _value_b -= 6;
      }
    }
    if (_value_b > -6)
      _output.push_back(
          BASE64URL_CHARSET[((_value << 8) >> (_value_b + 8)) & 0x3F]);

    if (padding)
      while (_output.size() % 4 != 0) _output.push_back('=');

    return _output;
  }

  /**
   * Converts from Base64url
   *
   * @param input
   * @return string
   */
  inline std::string base64url_decode(const std::string &input) noexcept {
    std::string _output;
    int _value = 0;
    int _value_b = -8;
    for (const char _character : input) {
      if (_character == '=') break;
      const int _in_map_value = BASE64URL_CHARSET_MAP[static_cast<unsigned char>(_character)];
      if (_in_map_value == -1) continue;
      _value = (_value << 6) + _in_map_value;
      _value_b += 6;
      if (_value_b >= 0) {
        _output.push_back(static_cast<char>(_value >> _value_b & 0xFF));
        _value_b -= 8;
      }
    }
    return _output;
  }
}

#endif // FRAMEWORK_ENCODING_BASE64URL_HPP

// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.

#ifndef CRC_H
#define CRC_H

#include <struct_typedef.h>

/**
 * @brief CRC16 Caculation function
 * @param[in] pchMessage : Data to Verify,
 * @param[in] dwLength : Stream length = Data + checksum
 * @param[in] wCRC : CRC16 init value(default : 0xFFFF)
 * @return : CRC16 checksum
 */
uint16_t Get_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);

/**
 * @brief CRC16 Verify function
 * @param[in] pchMessage : Data to Verify,
 * @param[in] dwLength : Stream length = Data + checksum
 * @return : True or False (CRC Verify Result)
 */
uint8_t Verify_CRC16_Check_Sum(const uint8_t *pchMessage, uint32_t dwLength);

/**
 * @brief Append CRC16 value to the end of the buffer
 * @param[in] pchMessage : Data to Verify,
 * @param[in] dwLength : Stream length = Data + checksum
 * @return none
 */
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

#endif // RM_SERIAL_DRIVER__CRC_HPP_

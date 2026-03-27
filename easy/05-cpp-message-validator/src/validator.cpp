/**
 * MAVLink Message Validator - реализация валидатора MAVLink сообщений.
 * 
 * Задание: реализуйте методы класса MAVLinkValidator.
 */

#include "validator.h"
#include <cstring>

namespace mavlink {

// Таблица CRC extra значений
const std::unordered_map<uint8_t, uint8_t> MAVLinkValidator::crc_extra_table_ = {
    {0, 50},    // HEARTBEAT
    {1, 124},   // SYS_STATUS
    {24, 24},   // GPS_RAW_INT
    {30, 39},   // ATTITUDE
    {33, 104},  // GLOBAL_POSITION_INT
    {74, 20},   // VFR_HUD
    {76, 152},  // COMMAND_LONG
    {77, 143},  // COMMAND_ACK
};

// Константы MAVLink v1
constexpr uint8_t MAVLINK_STX_V1 = 0xFE;
constexpr size_t MAVLINK_HEADER_SIZE = 6;  // magic + len + seq + sys + comp + msg
constexpr size_t MAVLINK_CRC_SIZE = 2;


MAVLinkValidator::MAVLinkValidator()
    : expected_system_id_(0)
    , check_sequence_(false)
    , last_sequence_(0)
    , first_message_(true)
    , missed_count_(0) {
}

void MAVLinkValidator::setExpectedSystemId(uint8_t id) {
    expected_system_id_ = id;
}

void MAVLinkValidator::enableSequenceCheck(bool enable) {
    check_sequence_ = enable;
}

uint8_t MAVLinkValidator::getLastSequence() const {
    return last_sequence_;
}

uint32_t MAVLinkValidator::getMissedCount() const {
    return missed_count_;
}

void MAVLinkValidator::reset() {
    last_sequence_ = 0;
    first_message_ = true;
    missed_count_ = 0;
}

std::optional<uint8_t> MAVLinkValidator::getCrcExtra(uint8_t message_id) const {
    auto it = crc_extra_table_.find(message_id);
    if (it != crc_extra_table_.end()) {
        return it->second;
    }
    return std::nullopt;
}

uint16_t MAVLinkValidator::calculateCrc(const std::vector<uint8_t>& data, uint8_t crc_extra) const {
    // TODO: Реализуйте вычисление CRC
    //
    // Алгоритм X.25 CRC:
    // 1. Начальное значение CRC = 0xFFFF
    // 2. Для каждого байта данных: crc = crc_accumulate(byte, crc)
    // 3. В конце добавить crc_extra: crc = crc_accumulate(crc_extra, crc)
    //
    // Функция crc_accumulate:
    // uint8_t tmp = data ^ (crc & 0xFF);
    // tmp ^= (tmp << 4);
    // return (crc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4);
    
    uint16_t crc = 0xFFFF;
    
    // Ваш код здесь
    
    return crc;
}

std::optional<MAVLinkMessage> MAVLinkValidator::parse(const std::vector<uint8_t>& data) {
    // TODO: Реализуйте парсинг MAVLink пакета
    //
    // Шаги:
    // 1. Проверить минимальную длину данных (HEADER + CRC = 8 байт)
    // 2. Извлечь magic (байт 0)
    // 3. Извлечь length (байт 1)
    // 4. Проверить что данных достаточно (HEADER + length + CRC)
    // 5. Извлечь остальные поля заголовка
    // 6. Извлечь payload
    // 7. Извлечь CRC (little-endian)
    //
    // Вернуть nullopt если данные некорректны
    
    // Ваш код здесь
    
    return std::nullopt;
}

ValidationResult MAVLinkValidator::validate(const MAVLinkMessage& msg) {
    // TODO: Реализуйте валидацию сообщения
    //
    // Проверки в порядке приоритета:
    // 1. Magic byte должен быть 0xFE
    // 2. Message ID должен быть в таблице crc_extra_table_
    // 3. CRC должна совпадать с вычисленной
    // 4. System ID должен совпадать (если expected_system_id_ != 0)
    // 5. Sequence должен быть последовательным (если check_sequence_ == true)
    //
    // При проверке sequence:
    // - Первое сообщение всегда валидно
    // - expected = (last_sequence_ + 1) % 256
    // - Если sequence != expected, увеличить missed_count_
    // - Обновить last_sequence_
    
    // Ваш код здесь
    
    return ValidationResult::Valid;
}

ValidationResult MAVLinkValidator::validateRaw(const std::vector<uint8_t>& data) {
    // TODO: Реализуйте комбинированную функцию
    //
    // 1. Вызвать parse()
    // 2. Если parse вернул nullopt, вернуть InvalidLength
    // 3. Вызвать validate() на результате
    
    // Ваш код здесь
    
    return ValidationResult::InvalidLength;
}

}  // namespace mavlink

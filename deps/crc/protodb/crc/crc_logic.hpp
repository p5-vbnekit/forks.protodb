#pragma once

#include <cstdint>
#include <array>
#include <variant>
#include <limits>

namespace ant::api {

using Crc = std::uint64_t;

struct CrcModel {
    std::size_t width;
    Crc poly;
    Crc seed;
    Crc xor_out;
    bool ref_in;
    bool ref_out;
};

class CrcLogic final {
public:
    static constexpr std::size_t k_table_size    = 16u;
    static constexpr std::size_t k_min_poly_size = 3u;
    static constexpr std::size_t k_max_poly_size = 64u;

public:
    CrcLogic();
    CrcLogic(const CrcModel& a_model);

    CrcModel model();
    bool setModel(const CrcModel& a_model);
    bool validateModel(const CrcModel& a_model);

    Crc finalize();
    std::size_t validBytes();
    void calculate(const std::uint8_t* data, const std::uint8_t* end);

private:
    std::size_t m_width;
    Crc m_poly;
    Crc m_seed;
    Crc m_xor_out;
    Crc m_result_mask;
    bool m_ref_in;
    bool m_ref_out;

private:
    std::array< Crc, k_table_size > m_table;
    Crc m_crc;

    void calculateTable();
};

}  // namespace ant::api


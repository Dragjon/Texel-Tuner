#include "toy_tapered.h"
#include "toy_base.h"
#include "../external/chess.hpp"

#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>



using namespace std;
using namespace Toy;
using namespace chess;

struct Trace
{
    int32_t material[6][2]{};
    int32_t pst[6][64][2]{};
    int32_t bishop_pair[2]{};
    int32_t mobility[6][2]{};
};

constexpr std::array<int32_t, 6> material = {S(69, 118), S(299, 354), S(313, 358), S(401, 654), S(829, 1240), S(0, 0)};
//constexpr std::array<int32_t, 6> mobility = {S(16, 17), S(4, 5), S(5, 6), S(4, 5), S(3, 4), S(5, 6)};
constexpr std::array<std::array<int32_t, 64>, 6> pst =
{{
    {{
        S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0),
        S(75, 181), S(99, 173), S(75, 174), S(106, 124), S(89, 118), S(71, 130), S(1, 177), S(-26, 192),
        S(-7, 119), S(7, 128), S(41, 92), S(47, 70), S(50, 61), S(73, 45), S(53, 94), S(8, 92),
        S(-23, 45), S(3, 35), S(6, 14), S(8, 5), S(30, -4), S(22, -1), S(26, 19), S(1, 19),
        S(-34, 20), S(-5, 17), S(-7, -1), S(10, -4), S(11, -7), S(3, -5), S(11, 7), S(-12, -0),
        S(-35, 13), S(-10, 16), S(-10, -3), S(-8, 10), S(7, 3), S(-5, -1), S(26, 5), S(-5, -4),
        S(-35, 18), S(-9, 20), S(-14, 5), S(-24, 13), S(-3, 17), S(12, 5), S(36, 5), S(-12, -3),
        S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0), S(0, 0),
    }},

    {{
        S(-163, -72), S(-130, -12), S(-62, 1), S(-27, -6), S(6, -3), S(-53, -28), S(-111, -6), S(-104, -98),
        S(-26, -20), S(-8, 3), S(19, 12), S(34, 11), S(18, 2), S(84, -14), S(-9, -1), S(14, -36),
        S(-11, -3), S(24, 14), S(42, 31), S(54, 32), S(92, 15), S(93, 9), S(49, 2), S(16, -14),
        S(-15, 10), S(-2, 31), S(24, 42), S(46, 45), S(27, 46), S(52, 39), S(9, 30), S(20, 1),
        S(-29, 12), S(-13, 21), S(2, 45), S(3, 45), S(13, 48), S(7, 38), S(5, 22), S(-18, 2),
        S(-49, -5), S(-25, 15), S(-11, 25), S(-8, 39), S(3, 37), S(-7, 21), S(-3, 9), S(-32, -4),
        S(-62, -15), S(-50, 2), S(-33, 12), S(-21, 16), S(-20, 15), S(-17, 9), S(-31, -8), S(-34, -4),
        S(-107, -22), S(-52, -35), S(-65, -4), S(-50, -1), S(-46, 1), S(-32, -12), S(-49, -27), S(-76, -36),
    }},

        {{
        S(-41, 10), S(-58, 20), S(-49, 18), S(-93, 31), S(-79, 25), S(-62, 15), S(-33, 11), S(-69, 5),
        S(-24, -6), S(1, 15), S(-6, 19), S(-23, 23), S(8, 12), S(7, 13), S(-2, 19), S(-13, -7),
        S(-13, 24), S(11, 20), S(11, 32), S(35, 19), S(21, 25), S(54, 25), S(31, 18), S(17, 17),
        S(-22, 20), S(-8, 37), S(15, 32), S(25, 47), S(23, 37), S(19, 36), S(-7, 33), S(-21, 19),
        S(-30, 15), S(-15, 34), S(-9, 43), S(12, 38), S(9, 40), S(-7, 37), S(-15, 31), S(-21, 5),
        S(-18, 14), S(-11, 26), S(-11, 33), S(-8, 34), S(-7, 38), S(-12, 35), S(-9, 17), S(-5, 5),
        S(-16, 10), S(-15, 9), S(-3, 8), S(-26, 23), S(-18, 25), S(-5, 13), S(2, 17), S(-11, -11),
        S(-40, -9), S(-17, 10), S(-34, -11), S(-43, 11), S(-39, 8), S(-39, 8), S(-14, -5), S(-28, -23),
    }},

        {{
        S(35, 26), S(24, 34), S(31, 43), S(37, 39), S(56, 30), S(75, 19), S(54, 22), S(77, 15),
        S(15, 27), S(15, 38), S(34, 43), S(54, 34), S(40, 34), S(70, 19), S(57, 15), S(88, 0),
        S(-7, 27), S(15, 29), S(17, 31), S(20, 29), S(50, 16), S(51, 9), S(90, 1), S(66, -5),
        S(-23, 29), S(-8, 27), S(-7, 37), S(2, 33), S(8, 17), S(10, 11), S(19, 7), S(21, 1),
        S(-43, 22), S(-40, 26), S(-30, 28), S(-16, 27), S(-17, 22), S(-32, 21), S(-8, 7), S(-16, 2),
        S(-50, 17), S(-40, 17), S(-31, 16), S(-32, 21), S(-26, 16), S(-28, 8), S(8, -13), S(-15, -13),
        S(-53, 11), S(-40, 15), S(-25, 16), S(-28, 19), S(-23, 9), S(-21, 5), S(-3, -5), S(-34, 0),
        S(-33, 7), S(-31, 16), S(-21, 25), S(-16, 23), S(-12, 15), S(-22, 10), S(-7, 5), S(-31, -3),
    }},

        {{
        S(-39, 24), S(-33, 40), S(-1, 58), S(34, 43), S(33, 41), S(38, 33), S(59, -14), S(4, 18),
        S(-1, -12), S(-24, 33), S(-18, 69), S(-26, 88), S(-19, 106), S(20, 63), S(-1, 45), S(44, 19),
        S(-1, -1), S(-3, 19), S(-6, 62), S(11, 64), S(16, 79), S(59, 58), S(60, 19), S(58, 5),
        S(-18, 11), S(-14, 35), S(-10, 51), S(-11, 74), S(-9, 89), S(5, 73), S(5, 58), S(11, 35),
        S(-17, 8), S(-19, 39), S(-19, 47), S(-11, 69), S(-12, 65), S(-13, 57), S(-1, 36), S(2, 23),
        S(-19, -4), S(-12, 14), S(-17, 38), S(-18, 34), S(-14, 38), S(-7, 31), S(6, 7), S(-1, -6),
        S(-21, -9), S(-15, -4), S(-5, -8), S(-5, 2), S(-7, 6), S(2, -22), S(9, -52), S(20, -82),
        S(-24, -14), S(-34, -7), S(-27, -4), S(-11, -13), S(-20, -8), S(-34, -8), S(-9, -42), S(-18, -40),
    }},

        {{
        S(37, -105), S(11, -52), S(47, -42), S(-102, 10), S(-45, -11), S(8, -8), S(63, -18), S(173, -128),
        S(-90, -7), S(-46, 23), S(-91, 37), S(21, 17), S(-33, 39), S(-29, 51), S(13, 40), S(-11, 8),
        S(-109, 9), S(-2, 28), S(-72, 48), S(-92, 60), S(-51, 59), S(29, 51), S(8, 49), S(-30, 19),
        S(-75, -2), S(-88, 34), S(-103, 53), S(-150, 66), S(-138, 66), S(-98, 59), S(-97, 51), S(-122, 24),
        S(-66, -14), S(-79, 18), S(-110, 44), S(-138, 59), S(-136, 58), S(-98, 44), S(-101, 32), S(-127, 15),
        S(-22, -25), S(-6, -0), S(-65, 22), S(-78, 35), S(-72, 34), S(-69, 25), S(-21, 4), S(-39, -8),
        S(72, -48), S(28, -19), S(13, -6), S(-23, 6), S(-25, 9), S(-6, -1), S(45, -21), S(55, -39),
        S(66, -84), S(91, -64), S(63, -44), S(-42, -24), S(26, -51), S(-16, -26), S(70, -54), S(72, -84)
    }},
}};

constexpr int32_t bishop_pair = S(21, 68);
constexpr std::array<int32_t, 6> mobility = {S(68, 116), S(295, 349), S(310, 353), S(394, 643), S(815, 1219), S(0, 0)};

static Trace trace_evaluate(const Position& position)
{
    Trace trace{};
    std::array<int, 2> bishop_counts{};

    for (int i = 0; i < 64; i++)
    {
        auto piece = position.pieces[i];

        if (piece == Pieces::None)
        {
            continue;
        }

        Bitboard atks;
        switch (piece.type()) {
          case PieceType::PAWN:
            atks = chess::attacks::pawn(piece.color(), i);
            break;
          case PieceType::KNIGHT:
            atks = chess::attacks::knight(i);
            break;
          case PieceType::BISHOP:
            atks = chess::attacks::bishop(i, position.occ());
            break;
          case PieceType::ROOK:
            atks = chess::attacks::rook(i, position.occ());
            break;
          case PieceType::QUEEN:
            atks = chess::attacks::queen(i, position.occ());
            break;
          case PieceType::KING:
            atks = chess::attacks::king(i);
            break;
        }
        int numAttacks = atks.count();


        if (piece < Pieces::BlackPawn)
        {
            const int materialIndex = static_cast<int>(piece) - static_cast<int>(Pieces::WhitePawn);
            trace.material[materialIndex][0]++;
            trace.mobility[materialIndex][0] += numAttacks;
            trace.pst[materialIndex][i^56][0]++;
            if (piece == Pieces::WhiteBishop)
            {
                bishop_counts[0]++;
            }

        }
        else
        {
            const int materialIndex = static_cast<int>(piece) - static_cast<int>(Pieces::BlackPawn);
            trace.material[materialIndex][1]++;
            trace.mobility[materialIndex][1] += numAttacks;
            trace.pst[materialIndex][i][1]++;

            if (piece == Pieces::BlackBishop)
            {
                bishop_counts[1]++;
            }

        }
    }

    for (int color = 0; color < 2; color++)
    {
        trace.bishop_pair[color] += bishop_counts[color] == 2;
    }

    return trace;
}

static coefficients_t get_coefficients(const Trace& trace)
{
    coefficients_t coefficients;
    get_coefficient_array(coefficients, trace.material, 6);
    get_coefficient_array_2d(coefficients, trace.pst, 6, 64);
    get_coefficient_single(coefficients, trace.bishop_pair);
    get_coefficient_array(coefficients, trace.mobility, 6);
    return coefficients;
}

parameters_t ToyEvalTapered::get_initial_parameters()
{
    parameters_t parameters;
    get_initial_parameter_array(parameters, material, 6);
    get_initial_parameter_array_2d(parameters, pst, 6, 64);
    get_initial_parameter_single(parameters, bishop_pair);
    get_initial_parameter_array(parameters, mobility, 6);
    return parameters;
}

EvalResult ToyEvalTapered::get_fen_eval_result(const string& fen)
{
    Position position;
    parse_fen(fen, position);
    auto trace = trace_evaluate(position);
    EvalResult result;
    result.coefficients = get_coefficients(trace);
    result.score = 0;
    return result;
}

EvalResult ToyEvalTapered::get_external_eval_result(const chess::Board& board)
{
    throw std::runtime_error("Not implemented");
}

static void print_parameter(std::stringstream& ss, const pair_t parameter)
{
    ss << "S(" << std::round(parameter[static_cast<int32_t>(PhaseStages::Midgame)]) << ", "
       << std::round(parameter[static_cast<int32_t>(PhaseStages::Endgame)]) << ")";
}

static void print_single(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name)
{
    ss << "constexpr int " << name << " = ";
    print_parameter(ss, parameters[index]);
    ss << ";" << std::endl;
    index++;
}

static void print_array(std::stringstream& ss, const parameters_t& parameters, int& index, const std::string& name, int count)
{
    ss << "constexpr int " << name << "[] = {";
    for (auto i = 0; i < count; i++)
    {
        print_parameter(ss, parameters[index]);
        index++;

        if (i != count - 1)
        {
            ss << ", ";
        }
    }
    ss << "};" << std::endl;
}

void ToyEvalTapered::print_parameters(const parameters_t& parameters)
{
    int index = 0;
    stringstream ss;
    print_array(ss, parameters, index, "material", 6);
    print_array(ss, parameters, index, "pst", 384);
    print_single(ss, parameters, index, "bishop_pair");
    print_array(ss, parameters, index, "mobility", 6);
    cout << ss.str() << "\n";
}

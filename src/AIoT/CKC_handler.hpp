#ifndef INC_CKC_VIRTUAL_PIN
#define INC_CKC_VIRTUAL_PIN

#pragma once

#include <stdint.h>

// #define CKC_100_PINS

// ===== Forward =====
class CKCParam;

#define V0 0
#define V1 1
#define V2 2
#define V3 3
#define V4 4
#define V5 5
#define V6 6
#define V7 7
#define V8 8
#define V9 9
#define V10 10
#define V11 11
#define V12 12
#define V13 13
#define V14 14
#define V15 15
#define V16 16
#define V17 17
#define V18 18
#define V19 19
#define V20 20
#define V21 21
#define V22 22
#define V23 23
#define V24 24
#define V25 25
#define V26 26
#define V27 27
#define V28 28
#define V29 29
#define V30 30
#define V31 31
#define V32 32
#define V33 33
#define V34 34
#define V35 35
#define V36 36
#define V37 37
#define V38 38
#define V39 39
#define V40 40
#define V41 41
#define V42 42
#define V43 43
#define V44 44
#define V45 45
#define V46 46
#define V47 47
#define V48 48
#define V49 49
#if defined(CKC_100_PINS)
#define V50 50
#define V51 51
#define V52 52
#define V53 53
#define V54 54
#define V55 55
#define V56 56
#define V57 57
#define V58 58
#define V59 59
#define V60 60
#define V61 61
#define V62 62
#define V63 63
#define V64 64
#define V65 65
#define V66 66
#define V67 67
#define V68 68
#define V69 69
#define V70 70
#define V71 71
#define V72 72
#define V73 73
#define V74 74
#define V75 75
#define V76 76
#define V77 77
#define V78 78
#define V79 79
#define V80 80
#define V81 81
#define V82 82
#define V83 83
#define V84 84
#define V85 85
#define V86 86
#define V87 87
#define V88 88
#define V89 89
#define V90 90
#define V91 91
#define V92 92
#define V93 93
#define V94 94
#define V95 95
#define V96 96
#define V97 97
#define V98 98
#define V99 99
#endif

// ===== Callback type =====
typedef void (*handlerWidget_pin)(uint8_t pin, const CKCParam& param);

// ===== User define macro =====
#define CKC_WRITE_2(Pin) \
    void CKC_WidgetWrite##Pin(uint8_t pin, const CKCParam& param)
#define CKC_WRITE(Pin) CKC_WRITE_2(Pin)

// ======================================================
// ===== DANH SÁCH VIRTUAL PIN (CHỈ SỬA CHỖ NÀY) =====
// ======================================================
#define CKC_VPIN_LIST \
X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) \
X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) \
X(20) X(21) X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) \
X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39) \
X(40) X(41) X(42) X(43) X(44) X(45) X(46) X(47) X(48) X(49) \
X(50) X(51) X(52) X(53) X(54) X(55) X(56) X(57) X(58) X(59) \
X(60) X(61) X(62) X(63) X(64) X(65) X(66) X(67) X(68) X(69) \
X(70) X(71) X(72) X(73) X(74) X(75) X(76) X(77) X(78) X(79) \
X(80) X(81) X(82) X(83) X(84) X(85) X(86) X(87) X(88) X(89) \
X(90) X(91) X(92) X(93) X(94) X(95) X(96) X(97) X(98) X(99) 

// ===== Auto extern =====
#define X(n) extern void CKC_WidgetWrite##n(uint8_t, const CKCParam&);
CKC_VPIN_LIST
#undef X

// ===== Vector + count =====
extern const handlerWidget_pin CKC_HandlerVector[];
extern const uint8_t CKC_HandlerCount;

// ===== Default fallback =====
void CKC_WidgetWrite_Default(uint8_t pin, const CKCParam& param);


#endif
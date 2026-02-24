#ifndef INC_CKC_PIN
#define INC_CKC_PIN

#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>

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
#if defined(ERA_100_PINS)
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
#if defined(ERA_255_PINS)
#define V100 100
#define V101 101
#define V102 102
#define V103 103
#define V104 104
#define V105 105
#define V106 106
#define V107 107
#define V108 108
#define V109 109
#define V110 110
#define V111 111
#define V112 112
#define V113 113
#define V114 114
#define V115 115
#define V116 116
#define V117 117
#define V118 118
#define V119 119
#define V120 120
#define V121 121
#define V122 122
#define V123 123
#define V124 124
#define V125 125
#define V126 126
#define V127 127
#define V128 128
#define V129 129
#define V130 130
#define V131 131
#define V132 132
#define V133 133
#define V134 134
#define V135 135
#define V136 136
#define V137 137
#define V138 138
#define V139 139
#define V140 140
#define V141 141
#define V142 142
#define V143 143
#define V144 144
#define V145 145
#define V146 146
#define V147 147
#define V148 148
#define V149 149
#define V150 150
#define V151 151
#define V152 152
#define V153 153
#define V154 154
#define V155 155
#define V156 156
#define V157 157
#define V158 158
#define V159 159
#define V160 160
#define V161 161
#define V162 162
#define V163 163
#define V164 164
#define V165 165
#define V166 166
#define V167 167
#define V168 168
#define V169 169
#define V170 170
#define V171 171
#define V172 172
#define V173 173
#define V174 174
#define V175 175
#define V176 176
#define V177 177
#define V178 178
#define V179 179
#define V180 180
#define V181 181
#define V182 182
#define V183 183
#define V184 184
#define V185 185
#define V186 186
#define V187 187
#define V188 188
#define V189 189
#define V190 190
#define V191 191
#define V192 192
#define V193 193
#define V194 194
#define V195 195
#define V196 196
#define V197 197
#define V198 198
#define V199 199
#define V200 200
#define V201 201
#define V202 202
#define V203 203
#define V204 204
#define V205 205
#define V206 206
#define V207 207
#define V208 208
#define V209 209
#define V210 210
#define V211 211
#define V212 212
#define V213 213
#define V214 214
#define V215 215
#define V216 216
#define V217 217
#define V218 218
#define V219 219
#define V220 220
#define V221 221
#define V222 222
#define V223 223
#define V224 224
#define V225 225
#define V226 226
#define V227 227
#define V228 228
#define V229 229
#define V230 230
#define V231 231
#define V232 232
#define V233 233
#define V234 234
#define V235 235
#define V236 236
#define V237 237
#define V238 238
#define V239 239
#define V240 240
#define V241 241
#define V242 242
#define V243 243
#define V244 244
#define V245 245
#define V246 246
#define V247 247
#define V248 248
#define V249 249
#define V250 250
#define V251 251
#define V252 252
#define V253 253
#define V254 254
#define V255 255
#endif
#if defined(ERA_500_PINS)
#define V256 256
#define V257 257
#define V258 258
#define V259 259
#define V260 260
#define V261 261
#define V262 262
#define V263 263
#define V264 264
#define V265 265
#define V266 266
#define V267 267
#define V268 268
#define V269 269
#define V270 270
#define V271 271
#define V272 272
#define V273 273
#define V274 274
#define V275 275
#define V276 276
#define V277 277
#define V278 278
#define V279 279
#define V280 280
#define V281 281
#define V282 282
#define V283 283
#define V284 284
#define V285 285
#define V286 286
#define V287 287
#define V288 288
#define V289 289
#define V290 290
#define V291 291
#define V292 292
#define V293 293
#define V294 294
#define V295 295
#define V296 296
#define V297 297
#define V298 298
#define V299 299
#define V300 300
#define V301 301
#define V302 302
#define V303 303
#define V304 304
#define V305 305
#define V306 306
#define V307 307
#define V308 308
#define V309 309
#define V310 310
#define V311 311
#define V312 312
#define V313 313
#define V314 314
#define V315 315
#define V316 316
#define V317 317
#define V318 318
#define V319 319
#define V320 320
#define V321 321
#define V322 322
#define V323 323
#define V324 324
#define V325 325
#define V326 326
#define V327 327
#define V328 328
#define V329 329
#define V330 330
#define V331 331
#define V332 332
#define V333 333
#define V334 334
#define V335 335
#define V336 336
#define V337 337
#define V338 338
#define V339 339
#define V340 340
#define V341 341
#define V342 342
#define V343 343
#define V344 344
#define V345 345
#define V346 346
#define V347 347
#define V348 348
#define V349 349
#define V350 350
#define V351 351
#define V352 352
#define V353 353
#define V354 354
#define V355 355
#define V356 356
#define V357 357
#define V358 358
#define V359 359
#define V360 360
#define V361 361
#define V362 362
#define V363 363
#define V364 364
#define V365 365
#define V366 366
#define V367 367
#define V368 368
#define V369 369
#define V370 370
#define V371 371
#define V372 372
#define V373 373
#define V374 374
#define V375 375
#define V376 376
#define V377 377
#define V378 378
#define V379 379
#define V380 380
#define V381 381
#define V382 382
#define V383 383
#define V384 384
#define V385 385
#define V386 386
#define V387 387
#define V388 388
#define V389 389
#define V390 390
#define V391 391
#define V392 392
#define V393 393
#define V394 394
#define V395 395
#define V396 396
#define V397 397
#define V398 398
#define V399 399
#define V400 400
#define V401 401
#define V402 402
#define V403 403
#define V404 404
#define V405 405
#define V406 406
#define V407 407
#define V408 408
#define V409 409
#define V410 410
#define V411 411
#define V412 412
#define V413 413
#define V414 414
#define V415 415
#define V416 416
#define V417 417
#define V418 418
#define V419 419
#define V420 420
#define V421 421
#define V422 422
#define V423 423
#define V424 424
#define V425 425
#define V426 426
#define V427 427
#define V428 428
#define V429 429
#define V430 430
#define V431 431
#define V432 432
#define V433 433
#define V434 434
#define V435 435
#define V436 436
#define V437 437
#define V438 438
#define V439 439
#define V440 440
#define V441 441
#define V442 442
#define V443 443
#define V444 444
#define V445 445
#define V446 446
#define V447 447
#define V448 448
#define V449 449
#define V450 450
#define V451 451
#define V452 452
#define V453 453
#define V454 454
#define V455 455
#define V456 456
#define V457 457
#define V458 458
#define V459 459
#define V460 460
#define V461 461
#define V462 462
#define V463 463
#define V464 464
#define V465 465
#define V466 466
#define V467 467
#define V468 468
#define V469 469
#define V470 470
#define V471 471
#define V472 472
#define V473 473
#define V474 474
#define V475 475
#define V476 476
#define V477 477
#define V478 478
#define V479 479
#define V480 480
#define V481 481
#define V482 482
#define V483 483
#define V484 484
#define V485 485
#define V486 486
#define V487 487
#define V488 488
#define V489 489
#define V490 490
#define V491 491
#define V492 492
#define V493 493
#define V494 494
#define V495 495
#define V496 496
#define V497 497
#define V498 498
#define V499 499
#endif

#pragma
class CKCParam
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        INT,
        FLOAT,
        BOOL,
        STRING
    };

    CKCParam() : type(Type::NONE) {}

    // Constructors (tự nhận kiểu)
    CKCParam(int v) { set(v); }
    CKCParam(float v) { set(v); }
    CKCParam(bool v) { set(v); }
    CKCParam(const char *v) { set(v); }
    CKCParam(const String &v) { set(v); }

    // set()
    void set(int v)
    {
        type = Type::INT;
        data.i = v;
    }

    void set(float v)
    {
        type = Type::FLOAT;
        data.f = v;
    }

    void set(bool v)
    {
        type = Type::BOOL;
        data.b = v;
    }

    void set(const char *v)
    {
        type = Type::STRING;
        str = v;
    }

    void set(const String &v)
    {
        type = Type::STRING;
        str = v;
    }

    // get type
    Type getType() const { return type; }

    // get value (an toàn)
    int getInt(int def = 0) const
    {
        if (type == Type::INT)
            return data.i;
        if (type == Type::BOOL)
            return data.b ? 1 : 0;
        if (type == Type::FLOAT)
            return (int)data.f;
        if (type == Type::STRING)
            return str.toInt();
        return def;
    }

    float getFloat(float def = 0) const
    {
        if (type == Type::FLOAT)
            return data.f;
        if (type == Type::INT)
            return (float)data.i;
        if (type == Type::BOOL)
            return data.b ? 1.0f : 0.0f;
        if (type == Type::STRING)
            return str.toFloat();
        return def;
    }

    bool getBool(bool def = false) const
    {
        if (type == Type::BOOL)
            return data.b;
        if (type == Type::INT)
            return data.i != 0;
        if (type == Type::FLOAT)
            return data.f != 0;
        if (type == Type::STRING)
            return (str == "1" || str == "true" || str == "TRUE");
        return def;
    }

    String getString(const String &def = "") const
    {
        if (type == Type::STRING)
            return str;
        if (type == Type::INT)
            return String(data.i);
        if (type == Type::FLOAT)
            return String(data.f);
        if (type == Type::BOOL)
            return data.b ? "true" : "false";
        return def;
    }

private:
    Type type;

    union
    {
        int i;
        float f;
        bool b;
    } data;

    String str; // string tách riêng vì union không chứa object được
};

#ifndef CKC_UNUSED
  #define CKC_UNUSED __attribute__((__unused__))
#endif

#define CKC_WRITE(Pin)      void CKC_WidgetWrite##Pin(uint8_t CKC_UNUSED &pin, const CKCParam CKC_UNUSED &CKCparam)

void CKC_WidgetWrite(uint8_t ERA_UNUSED &pin, const ERaParam ERA_UNUSED &param)
{

}

CKC_WRITE(V0)
{
    
}

#endif
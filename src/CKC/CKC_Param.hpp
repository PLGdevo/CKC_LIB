#ifndef INC_CKC_Param
#define INC_CKC_Param
#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>
#include <CKC/CKC_topic.h>

#pragma
class CKCParam
{
public:
    enum class Type : uint8_t
    {
        NONE = 0,
        INT,
        FLOAT,
        DOUBLE,
        BOOL,
        STRING
    };

    // Constructor mặc định
    CKCParam() : type(Type::NONE) {}

    // Constructors
    CKCParam(int v) { set(v); }
    CKCParam(float v) { set(v); }
    CKCParam(double v) { set(v); }
    CKCParam(bool v) { set(v); }
    CKCParam(const char *v) { set(v); }
    CKCParam(const String &v) { set(v); }

    // ===== SET =====
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

    void set(double v)
    {
        type = Type::DOUBLE;
        data.d = v;
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

    // ===== GET TYPE =====
    Type getType() const { return type; }

    // ===== GET INT =====
    int getInt(int def = 0) const
    {
        if (type == Type::INT)
            return data.i;
        if (type == Type::FLOAT)
            return (int)data.f;
        if (type == Type::DOUBLE)
            return (int)data.d;
        if (type == Type::BOOL)
            return data.b ? 1 : 0;
        if (type == Type::STRING)
            return str.toInt();
        return def;
    }

    // ===== GET FLOAT =====
    float getFloat(float def = 0) const
    {
        if (type == Type::FLOAT)
            return data.f;
        if (type == Type::DOUBLE)
            return (float)data.d;
        if (type == Type::INT)
            return (float)data.i;
        if (type == Type::BOOL)
            return data.b ? 1.0f : 0.0f;
        if (type == Type::STRING)
            return str.toFloat();
        return def;
    }

    // ===== GET DOUBLE =====
    double getDouble(double def = 0) const
    {
        if (type == Type::DOUBLE)
            return data.d;
        if (type == Type::FLOAT)
            return (double)data.f;
        if (type == Type::INT)
            return (double)data.i;
        if (type == Type::BOOL)
            return data.b ? 1.0 : 0.0;
        if (type == Type::STRING)
            return str.toDouble(); // ESP32 OK
        return def;
    }

    // ===== GET BOOL =====
    bool getBool(bool def = false) const
    {
        if (type == Type::BOOL)
            return data.b;
        if (type == Type::INT)
            return data.i != 0;
        if (type == Type::FLOAT)
            return data.f != 0;
        if (type == Type::DOUBLE)
            return data.d != 0;
        if (type == Type::STRING)
            return (str == "1" || str == "true" || str == "TRUE");
        return def;
    }

    // ===== GET STRING =====
    String getString(const String &def = "") const
    {
        if (type == Type::STRING)
            return str;
        if (type == Type::INT)
            return String(data.i);
        if (type == Type::FLOAT)
            return String(data.f);
        if (type == Type::DOUBLE)
            return String(data.d, 6); // giữ precision
        if (type == Type::BOOL)
            return data.b ? "true" : "false";
        return def;
    }

    // ===== META DATA (payload parse) =====
    void addSUB_PREFIX(String subprefix_H)
    {
        subprefix_h = subprefix_H;
    }

    void addpin_(int Pin_H)
    {
        Pin_h = Pin_H;
    }

    void addtype_(String type_H)
    {
        type_h = type_H;
    }

    void addvalue_(int value_H)
    {
        value_h = value_H;
    }

    // ===== GET META =====
    int getvalue_() { return value_h; }
    int getpin_() { return Pin_h; }
    String gettype_() { return type_h; }
    String getsubprefix() { return subprefix_h; }

private:
    Type type;

    union
    {
        int i;
        float f;
        double d; // thêm double
        bool b;
    } data;

    String str;

    // payload data
    String type_h, subprefix_h;
    int value_h = 0;
    int Pin_h = 0;
};

CKCParam parseItem(cJSON *item)
{
    CKCParam value;

    if (item == nullptr)
        return value;

    if (cJSON_IsNumber(item))
    {
        // phân biệt int vs float
        double val = item->valuedouble;
        if (val == (int)val)
        {
            value.set((int)val);
            // CKC_LOG_DEBUG("PARAM", "value int : %d", value.getInt());
        }
        else
        {
            value.set((float)val);            
            // CKC_LOG_DEBUG("PARAM", "value float : %f", value.getFloat());
        }
    }
    else if (cJSON_IsBool(item))
    {
        value.set(cJSON_IsTrue(item));
        value.getBool();
    }
    else if (cJSON_IsString(item))
    {
        value.set(item->valuestring);        
        // CKC_LOG_DEBUG("PARAM", "value string : %s", item->valuestring);
    }
    else
    {
        // fallback
    }

    return value;
}

#endif
#ifndef INC_CKC_Param
#define INC_CKC_Param
#include <stdio.h>
#include <stdint.h>
#include <Arduino.h>
#include <CkC/CKC_topic.h>

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
    void add(int v) { intVal = v; }
    // Constructors
    CKCParam(int v) { set(v); }
    CKCParam(float v) { set(v); }
    CKCParam(bool v) { set(v); }
    CKCParam(const char *v) { set(v); }
    CKCParam(const String &v) { set(v); }

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

    // get type data
    Type getType() const { return type; }

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

    // add SUB_PREFIX after parse
    void addSUB_PREFIX(String subprefix_H)
    {

        this->subprefix_h = subprefix_H;
    }
    // add payload after parse
    void addpin_(int Pin_H)
    {
        this->Pin_h = Pin_H;
    }
    void addtype_(String type_H)
    {
        this->type_h = type_H;
    }
    void addvalue_(int value_H)
    {
        this->value_h = value_H;
    }
    // get payload after parse
    int getvalue_()
    {
        return value_h;
    }
    int getpin_()
    {
        return Pin_h;
    }
    String gettype_()
    {
        return type_h;
    }
    String getsubprefix()
    {
        return subprefix_h;
    }

private:
    Type type;
    union
    {
        int i;
        float f;
        bool b;
    } data;
    int intVal;
    String str;

    String type_h, subprefix_h;
    int value_h, Pin_h;
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
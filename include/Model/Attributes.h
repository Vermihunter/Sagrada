#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

/**
 * Represents base class for attributes.
 *
 * Since in a lot of cases the only thing making
 * difference between using the color or value attributes
 * is the count of them or the type, it makes sense to
 * generalize these attributes.
 */
struct Attribute
{
};

/**
 * Represents the values that are used in the game.
 *
 * These values represent dice values that can occur
 * in form of a restriction on the WPC or a die.
 */
struct Value : Attribute
{
    /**
     * The concrete types it can have - no sanitization needed this way.
     */
    enum class TYPE
    {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        COUNT
    };
    static constexpr size_t SIZE = size_t(TYPE::COUNT);

    static bool val_is_correct(int val);
    /**
     * Hence a lot of components have a value attribute,
     * a generic getter is defined.
     */
    /* get_value() returns currently optional -> maybe change the whole system
     * to optional*/
    template <typename T>
    static TYPE get(T* t)
    {
        if (!t) {
            return TYPE::COUNT;
        }

        return t->get_value();
    }
};

struct Color : Attribute
{
    /**
     * The whole logic of the game uses these TYPEs for representing
     * colors, the mapping to an RGBA value happens only at the
     * last = visualization layer.
     */
    enum class TYPE
    {
        RED,
        BLUE,
        GREEN,
        YELLOW,
        PURPLE,
        COUNT
    };

    static constexpr size_t SIZE = size_t(TYPE::COUNT);

    static std::vector<TYPE> all_colors()
    {
        std::vector<TYPE> result;
        for (int i = 0; i < int(TYPE::COUNT); ++i) {
            result.push_back(TYPE(i));
        }
        return result;
    }

    /**
     * Maps a predefined type to an RGBA color
     */
    static Color& type_to_rgba(std::optional<TYPE> c);

    /**
     * Debug
     *
     * Cannot define operator<< because the type is not stored
     */
    static std::string type_to_string(std::optional<TYPE> c);

    /**
     * Hence a lot of components have a color attribute,
     * a generic getter is defined.
     */
    template <typename T>
    static TYPE get(T* t)
    {
        if (!t) {
            return TYPE::COUNT;
        }

        return t->get_color();
    }

    // #pragma GCC diagnostic push
    // #pragma GCC diagnostic ignored "-Wfloat-equal"
    //     friend bool operator==(const Color& lhs, const Color& rhs)
    //     {
    //         return lhs.r == rhs.r && lhs.g == rhs.g && rhs.b == rhs.b;
    //     }
    // #pragma GCC diagnostic push

    double r;
    double g;
    double b;
    double a;
};

using color_t = Color::TYPE;
using value_t = Value::TYPE;

/**
 * Predefined RGBA colors that contain both the colors represented
 * by the game logic and additional basic colors that are used for
 * visualization.
 */
extern Color red, green, blue, yellow, purple, white, grey, black, orange;

#endif // ATTRIBUTES_H

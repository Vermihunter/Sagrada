#ifndef NUMBER_ENTRY_H
#define NUMBER_ENTRY_H

#include <gtkmm.h>

#include <algorithm>
#include <cctype>

#include "InformationNotProvidedException.h"

class NumberEntry : public Gtk::Entry
{
  public:
    void on_insert_text(const Glib::ustring& text, int* position)
    {
        if (contains_only_numbers(text))
            Gtk::Entry::on_insert_text(text, position);
    }

    size_t get_underlying_number(const std::string& fieldName)
    {
        auto txt = get_text();
        if (txt.empty()) {
            throw InformationNotProvidedException {fieldName +
                                                   " config not provided!"};
        }

        return std::stoull(txt);
    }

  private:
    bool contains_only_numbers(const std::string& text)
    {
        return std::all_of(text.begin(), text.end(),
                           [](char c) { return std::isdigit(c); });
    }
};

#endif // NUMBER_ENTRY_H
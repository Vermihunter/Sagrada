#ifndef DEALLOCATOR_H
#define DEALLOCATOR_H

template <typename _It>
void free_all(_It begin, _It end)
{
    for (; begin != end; ++begin) {
        delete *begin;
    }
}

template <typename T, typename _It>
void free_all_but(_It begin, _It end, T* elementToKeep)
{
    for (; begin != end; ++begin) {
        if (*begin != elementToKeep) {
            delete *begin;
        }
    }
}

template <typename T, typename _It>
void free_all_with_ind_but(_It begin, _It end, T* elementToKeep)
{
    for (; begin != end; ++begin) {
        if (begin->first != elementToKeep) {
            delete begin->first;
        }
    }
}

template <typename _It>
void free_all_with_ind(_It begin, _It end)
{
    for (; begin != end; ++begin) {
        delete begin->first;
    }
}

#endif // DEALLOCATOR_H
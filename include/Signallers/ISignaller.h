#ifndef I_SIGNALLER_H
#define I_SIGNALLER_H

#include <algorithm>
#include <vector>

/**
 * @brief Observer pattern implementation
 *
 * @tparam SignallableType The type of the signallable interface
 */
template <typename SignallableType> struct ISignaller {
	void subscribe(SignallableType* observer)
	{
		auto observerIt = std::find(observers.begin(), observers.end(), observer);
		if (observerIt == observers.end()) {
			observers.push_back(observer);
		}
	}

	void unsubscribe(SignallableType* observer)
	{
		auto observerIt = std::find(observers.begin(), observers.end(), observer);
		if (observerIt != observers.end()) {
			observers.erase(observerIt);
		}
	}

  protected:
	std::vector<SignallableType*> observers;
};

#endif // I_SIGNALLER_H
/**
 * @file filter.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_FILTER_H_
#define INC_LOG_FILTER_H_

#include <stdint.h>

namespace Log {

class filter {
  public:
	filter(): 
        bitfield(0) {}

	filter(uint32_t filters_): 
        bitfield(filters_) {}


    inline uint32_t get_filter() const {
        return this->bitfield;
    }

    inline void set(uint32_t levels) {
		this->bitfield |= levels;
	}

	inline bool is_set(uint32_t levels) const {
		return (levels & this->bitfield) != 0;
	}

    inline bool is_set(filter& levels) const {
		return (levels.get_filter() & this->bitfield) != 0;
	}

  private:
	uint32_t bitfield;
};

}  // namespace Log

#endif  // INC_LOG_FILTER_H_

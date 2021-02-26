#pragma once

#include "TypeTraits.h"
#include "Allocator.h"
#include "IteratorBase.h"

#include "Aggregate.h"
#include "All.h"
#include "Any.h"
#include "Avg.h"
#include "Contains.h"
#include "Count.h"
#include "Distinct.h"
#include "Empty.h"
#include "Generate.h"
#include "Max.h"
#include "Min.h"
#include "MinMax.h"
#include "OrderBy.h"
#include "PairWise.h"
#include "Range.h"
#include "Repeat.h"
#include "Reverse.h"
#include "Select.h"
#include "SequenceEqual.h"
#include "Singleton.h"
#include "Skip.h"
#include "StlCollection.h"
#include "Sum.h"
#include "Take.h"
#include "TakeOne.h"
#include "ThenBy.h"
#include "Where.h"
#include "ZipWith.h"

namespace linq {

/**
 * \brief Call Select
 * \tparam Array Have iterator
 * \tparam Builder Have build(Array)
 * \param arr Array have iterator
 * \param builder Have build(arr)
 * \return result have const iterator
 * \code
 * int array[16]{};
 * auto newArray = array << Select([](auto value) { return static_cast<float>(value); });
 */
template<class Array, class Builder, typename = is_builder_t<Array, Builder>>
constexpr auto operator<<( Array&& arr, Builder&& builder )
{
    return builder.build( std::forward<Array>( arr ) );
}
}

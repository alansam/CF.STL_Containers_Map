//
//  maps.cpp
//  CF.STL_Containers_Map
//
//  Created by Alan Sampson on 3/25/21.
//
//  MARK: - Reference.
//  @see: https://en.cppreference.com/w/cpp/container

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <utility>
#include <compare>
#include <memory>
#include <type_traits>
#include <forward_list>
#include <span>
#include <map>
#include <vector>
#include <cassert>
#include <cstddef>
#include <cmath>

using namespace std::literals::string_literals;

//  MARK: - Definitions

//  MARK: - Local Constants.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace konst
namespace konst {

auto delimiter(char const dc = '-', size_t sl = 80) -> std::string const {
  auto const dlm = std::string(sl, dc);
  return dlm;
}

static
auto const dlm = delimiter();

static
auto const dot = delimiter('.');

} /* namespace konst */

#if (__cplusplus > 201707L)
#endif  /* (__cplusplus > 201707L) */

//  MARK: - Function Prototype.
auto C_map(int argc, const char * argv[]) -> decltype(argc);

//  MARK: - Implementation.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  std::cout << "CF.STL_Containers_Map\n"s;
  std::cout << "C++ Version: "s << __cplusplus << std::endl;

  std::cout << '\n' << konst::dlm << std::endl;
  C_map(argc, argv);

  return 0;
}

//  MARK: - C_map
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  MARK: namespace cmap
namespace cmap {
// print out a std::pair
template <class Os, class U, class V>
Os & operator<<(Os & os, std::pair<U, V> const & kvp) {
  return os << kvp.first << ':' << kvp.second;
}

  // print out a container
template <class Os, class Co>
Os & operator<<(Os & os, Co const & container) {
  os << "{"s;
  for (auto const & elmt : container) { os << ' ' << elmt; }
  os << " }\n"s;
  return os;
}

} /* namespace cmap */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace cmapch
namespace cmapch {

// until C++20 chrono operator<< ready
std::ostream & operator<<(std::ostream & os, std::chrono::year_month_day const & ymd) {
    return os << std::setfill('0') << static_cast<int>(ymd.year()) << '/'
              << std::setw(2) << static_cast<unsigned>(ymd.month()) << '/'
              << std::setw(2) << static_cast<unsigned>(ymd.day())
              << std::setfill(' ');
}

} /* namespace cmapch */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace cmapfd
namespace cmapfd {

struct FatKey   {
  int x;
  int data[1000];
};

struct LightKey {
  int x;
};

// Note: as detailed above, the container must use std::less<> (or other
//   transparent Comparator) to access these overloads.
// This includes standard overloads, such as between std::string and std::string_view.
bool operator<(FatKey const & fk, LightKey const & lk) {
  return fk.x < lk.x;
}

bool operator<(LightKey const & lk, FatKey const & fk) {
  return lk.x < fk.x;
}

bool operator<(FatKey const & fk1, FatKey const & fk2) {
  return fk1.x < fk2.x;
}

} /* namespace cmapfd */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_map()
 */
auto C_map(int argc, const char * argv[]) -> decltype(argc) {
  std::cout << "In "s << __func__ << std::endl;

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map"s << '\n';
  {
    auto print_map = [](std::string_view comment,
                        std::map<std::string, int> const & map) {
      std::cout << comment << ' ';
      for (auto const & [key, value] : map) {
        std::cout << key << " = "s << value << "; "s;
      }
      std::cout << '\n';
    };

    // Create a map of three strings (that map to integers)
    std::map<std::string, int> map {
      { "CPU"s, 10 }, { "GPU"s, 15 }, { "RAM"s, 20 },
    };

    print_map("Initial map: "s, map);

    map["CPU"s] = 25;  // update an existing value
    map["SSD"s] = 30;  // insert a new value

    print_map("Updated map:"s, map);

    std::cout << '\n';
  }

  /// Member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - constructor"s << '\n';
  {
    auto print_map = [](auto & map) {
      std::cout << '{';
      for (auto & kvpair : map) {
        std::cout << kvpair.first << ':' << kvpair.second << ' ';
      }
      std::cout << "}\n"s;
    };

    struct Point {
      double x_val;
      double y_val;
    };
    struct PointCmp {
      bool operator()(Point const & lhs, Point const & rhs) const {
        return lhs.x_val < rhs.x_val; // NB. intentionally ignores y
      }
    };

    // (1) Default constructor
    std::map<std::string, int> map1;
    map1["something"s]  =  69;
    map1["anything"s]   = 199;
    map1["that thing"s] =  50;
    std::cout << "map1 = "s; print_map(map1);

    // (2) Range constructor
    std::map<std::string, int> iter(map1.find("anything"s), map1.end());
    std::cout << "\niter = "s; print_map(iter);
    std::cout << "map1 = "s; print_map(map1);

    // (3) Copy constructor
    std::map<std::string, int> copied(map1);
    std::cout << "\ncopied = "s; print_map(copied);
    std::cout << "map1 = "s; print_map(map1);

    // (4) Move constructor
    std::map<std::string, int> moved(std::move(map1));
    std::cout << "\nmoved = "s; print_map(moved);
    std::cout << "map1 = "s; print_map(map1);

    // (5) Initializer list constructor
    const std::map<std::string, int> init {
      { "this"s,  100 },
      { "can"s,   100 },
      { "be"s,    100 },
      { "const"s, 100 },
    };
    std::cout << "\ninit = "s; print_map(init);


    // Custom Key class option 1:
    // Use a comparison struct
    std::map<Point, double, PointCmp> mag = {
      { {  5, -12 }, 13 },
      { {  3,   4 },  5 },
      { { -8, -15 }, 17 }
    };

    for (auto pt : mag) {
        std::cout << "The magnitude of ("s << pt.first.x_val
                  << ", "s << pt.first.y_val << ") is "s
                  << pt.second << '\n';
    }

    // Custom Key class option 2:
    // Use a comparison lambda
    // This lambda sorts points according to their magnitudes, where note that
    //  these magnitudes are taken from the local variable mag
    auto cmpLambda = [&mag](const Point & lhs, const Point & rhs) {
      return mag[lhs] < mag[rhs];
    };

    //You could also use a lambda that is not dependent on local variables, like this:
    //auto cmpLambda = [](const Point &lhs, const Point &rhs) { return lhs.y < rhs.y; };
    std::map<Point, double, decltype(cmpLambda)> magy(cmpLambda);

    //Various ways of inserting elements:
    magy.insert(std::pair<Point, double>( { 5, -12 }, 13));
    magy.insert( { { 3, 4 }, 5 } );
    magy.insert( { Point { -8.0, -15.0 }, 17 } );

    std::cout << '\n';
    for(auto pt : magy)
        std::cout << "The magnitude of (" << pt.first.x_val
                  << ", " << pt.first.y_val << ") is "
                  << pt.second << '\n';

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - operator="s << '\n';
  {
    auto display_sizes = [](std::map<int, int> const & nums1,
                            std::map<int, int> const & nums2,
                            std::map<int, int> const & nums3) {
      std::cout << "nums1: "s << nums1.size()
                << " nums2: "s << nums2.size()
                << " nums3: "s << nums3.size() << '\n';
    };

    std::map<int, int> nums1 { { 3, 1 }, { 4, 1 }, { 5, 9 },
                               { 6, 1 }, { 7, 1 }, { 8, 9 } };
    std::map<int, int> nums2;
    std::map<int, int> nums3;

    std::cout << "Initially:\n"s;
    display_sizes(nums1, nums2, nums3);

    // copy assignment copies data from nums1 to nums2
    nums2 = nums1;

    std::cout << "After assigment:\n"s;
    display_sizes(nums1, nums2, nums3);

    // move assignment moves data from nums1 to nums3,
    // modifying both nums1 and nums3
    nums3 = std::move(nums1);

    std::cout << "After move assigment:\n"s;
    display_sizes(nums1, nums2, nums3);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - at"s << '\n';
  {

    auto keys = std::vector { "π"s, "e"s };
    auto mmap = std::map<std::string, double> {
      { keys[0], M_PI, }, { keys[1], M_E, },
    };

    try {
      std::for_each(keys.cbegin(), keys.cend(), [&mmap](auto key) {
        std::cout << key << ": "s << mmap.at(key) << '\n';
      });
    }
    catch (std::out_of_range & ex) {
      std::cerr << "bad key " << ex.what() << '\n';
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - operator[]"s << '\n';
  {
    std::map<char, int> letter_counts { { 'a', 27 }, { 'b', 3 }, { 'c', 1 }, };

    std::cout << "initially:\n"s;
    for (const auto &kvpair : letter_counts) {
      std::cout << kvpair.first << ": "s << kvpair.second << '\n';
    }

    letter_counts['b'] = 42;  // update an existing value
    letter_counts['x'] =  9;  // insert a new value

    std::cout << "after modifications:\n"s;

    for (const auto & kvpair : letter_counts) {
      std::cout << kvpair.first << ": "s << kvpair.second << '\n';
    }

    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    std::map<std::string, size_t>  word_map;
    for (const auto & word : {
      "this"s, "sentence"s, "is"s, "not"s, "a"s, "sentence"s,
      "this"s, "sentence"s, "is"s, "a"s, "hoax"s
    }) {
      ++word_map[word];
    }

    for (const auto & [word, count] : word_map) {
      std::cout << count << " occurrences of word '"s << word << "'\n"s;
    }

    std::cout << '\n';
  }

  /// Iterators
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - begin, end, cbegin, cend"s << '\n';
  {
    {
      std::map<int, float> num_map;
      num_map[4] = 4.13;
      num_map[9] = 9.24;
      num_map[1] = 1.09;
      // calls a_map.begin() and a_map.end()
      for (auto kvpair = num_map.begin(); kvpair != num_map.end(); ++kvpair) {
        std::cout << kvpair->first << ", "s << kvpair->second << '\n';
      }
    }

    /// Example using a custom comparison function
    {
      struct Point {
        double x_val;
        double y_val;
      };

      struct PointCmp {
        bool operator()(const Point * lhs, const Point * rhs) const {
          return lhs->x_val < rhs->x_val;
        }
      };

      //Note that although the x-coordinates are out of order, the
      // map will be iterated through by increasing x-coordinates
      Point points[3] = { { 2, 0 }, { 1, 0 }, { 3, 0 }, };

      //mag is a map sending the address of node to its magnitude in the x-y plane
      //Although the keys are pointers-to-Point, we want to order the map by the
      // x-coordinates of the points and NOT by the addresses of the Points. This
      // is done by using the PointCmp class's comparison method.
      std::map<Point *, double, PointCmp> mag( {
        { points,     2 },
        { points + 1, 1 },
        { points + 2, 3 },
      } );

      //Change each y-coordinate from 0 to the magnitude
      for (auto kvpair = mag.begin(); kvpair != mag.end(); ++kvpair) {
        auto cur = kvpair->first; // pointer to Node
        cur->y_val = mag[cur]; // could also have used  cur->y = iter->second;
      }

      //Update and print the magnitude of each node
      for (auto kvpair = mag.begin(); kvpair != mag.end(); ++kvpair) {
        auto cur = kvpair->first;
        mag[cur] = std::hypot(cur->x_val, cur->y_val);
        std::cout << "The magnitude of ("s << cur->x_val
                  << ", "s << cur->y_val << ") is "s;
        std::cout << kvpair->second << '\n';
      }

      //Repeat the above with the range-based for loop
      for (auto kvpair : mag) {
        auto cur = kvpair.first;
        cur->y_val = kvpair.second;
        mag[cur] = std::hypot(cur->x_val, cur->y_val);
        std::cout << "The magnitude of ("s << cur->x_val
                  << ", "s << cur->y_val << ") is ";
        std::cout << mag[cur] << '\n';
        // Note that in contrast to std::cout << iter->second << '\n'; above,
        // std::cout << i.second << '\n'; will NOT print the updated magnitude
        // If auto &i: mag was used instead, it will print the updated magnitude
      }
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - rbegin, rend, rcbegin, rcend"s << '\n';
  {
    {
      std::map<int, std::string_view> const coins {
        {  10, "dime"s        },
        { 100, "dollar"s      },
        {  50, "half dollar"s },
        {   5, "nickel"s      },
        {   1, "penny"s       },
        {  25, "quarter"s     }
      }; // initializer entries in name alphabetical order

      std::cout << "US coins in circulation, largest to smallest denomination:\n"s;
      for (auto it = coins.crbegin(); it != coins.crend(); ++it) {
        std::cout << std::setw(11) << it->second
                  << " = ¢"s << it->first << '\n';
      }
    }

    {
      using namespace cmapch;
      using namespace std::chrono;

      std::map<year_month_day, int> const messages {
        { February/17/2023 , 10 },
        { February/17/2023 , 20 },
        { February/16/2022 , 30 },
        { October/22/2022  , 40 },
        { June/14/2022     , 50 },
        { November/23/2021 , 60 },
        { December/10/2022 , 55 },
        { December/12/2021 , 45 },
        { April/1/2020     , 42 },
        { April/1/2020     , 24 },
      };

      std::cout << "Messages received, reverse date order:\n"s;
      for (auto kvpair = messages.crbegin(); kvpair != messages.crend(); ++kvpair) {
        std::cout << kvpair->first << " : "s << kvpair->second << '\n';
      }
    }

    std::cout << '\n';
  }

  /// Capacity
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - empty"s << '\n';
  {
    std::map<int, int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): "s << numbers.empty() << '\n';

    numbers.emplace(42, 13);
    numbers.insert(std::make_pair(13'317, 123));
    std::cout << "After adding elements, numbers.empty(): "s << numbers.empty() << '\n';
    std::cout << std::noboolalpha;

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - size"s << '\n';
  {
    std::map<int, char> nums { { 1, 'a' }, { 3, 'b' }, { 5, 'c' }, { 7, 'd' } };

    std::cout << "nums contains "s << nums.size() << " elements.\n"s;

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - max_size"s << '\n';
  {
    std::map<char, char> mapcs;
    std::cout << "Maximum size of a 'map' is "s << mapcs.max_size() << '\n';

    std::cout << '\n';
  }

  /// Modifiers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - clear"s << '\n';
  {
    std::map<int, char> container{ { 1, 'x' }, { 2, 'y' }, { 3, 'z' }, };

    auto print = [](std::pair<int, char> const & kvpair) {
      std::cout << " " << kvpair.first << '(' << kvpair.second << ')';
    };

    std::cout << "Before clear:"s;
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize="s << container.size() << '\n';

    std::cout << "Clear\n"s;
    container.clear();

    std::cout << "After clear:"s;
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize="s << container.size() << '\n';

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - insert"s << '\n';
  {
    auto printInsertionStatus = [](auto kvpair, bool success) {
      std::cout << "Insertion of "s << kvpair->first
                << (success ? " succeeded\n"s : " failed\n"s);
    };

    std::map<std::string, float> karasunoPlayerHeights;

    // Overload 3: insert from rvalue reference
    auto const [it_hinata, success] =
      karasunoPlayerHeights.insert( { "Hinata"s, 162.8 } );
    printInsertionStatus(it_hinata, success);

    {
      // Overload 1: insert from lvalue reference
      auto const [it, success2] =
        karasunoPlayerHeights.insert(*it_hinata);
      printInsertionStatus(it, success2);
    }

    {
      // Overload 2: insert via forwarding to emplace
      auto const [it, success] =
        karasunoPlayerHeights.insert( { "Kageyama", 180.6 } );
      printInsertionStatus(it, success);
    }

    {
      // Overload 6: insert from rvalue reference with positional hint
      std::size_t const nr = std::size(karasunoPlayerHeights);
      auto const it =
        karasunoPlayerHeights.insert(it_hinata, { "Azumane"s, 184.7 } );
      printInsertionStatus(it, std::size(karasunoPlayerHeights) != nr);
    }

    {
      // Overload 4: insert from lvalue reference with positional hint
      std::size_t const nr =
        std::size(karasunoPlayerHeights);
      auto const it = karasunoPlayerHeights.insert(it_hinata, *it_hinata);
      printInsertionStatus(it, std::size(karasunoPlayerHeights) != nr);
    }

    {
      // Overload 5: insert via forwarding to emplace with positional hint
      std::size_t const nr = std::size(karasunoPlayerHeights);
      auto const it =
        karasunoPlayerHeights.insert(it_hinata, { "Tsukishima", 188.3 } );
      printInsertionStatus(it, std::size(karasunoPlayerHeights) != nr);
    }

    auto node_hinata =
      karasunoPlayerHeights.extract(it_hinata);
    std::map<std::string, float> playerHeights;

    // Overload 7: insert from iterator range
    playerHeights.insert(std::begin(karasunoPlayerHeights),
                         std::end(karasunoPlayerHeights));

    // Overload 8: insert from initializer_list
    playerHeights.insert( { {"Kozume"s, 169.2}, {"Kuroo", 187.7}, } );


    // Overload 9: insert node
    auto const status = playerHeights.insert(std::move(node_hinata));
    printInsertionStatus(status.position, status.inserted);

    node_hinata = playerHeights.extract(status.position);
    {
      // Overload 10: insert node with positional hint
      std::size_t const nr = std::size(playerHeights);
      auto const it = playerHeights.insert(std::begin(playerHeights),
                                           std::move(node_hinata));
      printInsertionStatus(it, std::size(playerHeights) != nr);
    }


    // Print resulting map
    std::cout << std::left << '\n';
    for (auto const & [name, height] : playerHeights) {
      std::cout << std::setw(10) << name << " | "s << height << "cm\n"s;
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - insert_or_assign"s << '\n';
  {
    auto print_node = [](auto const & node) {
      std::cout << "["s << node.first << "] = "s << node.second << '\n';
    };

    auto print_result = [&print_node](auto const & pair) {
      std::cout << (pair.second ? "inserted: "s : "assigned: "s);
      print_node(*pair.first);
    };

    std::map<std::string, std::string> myMap;

    print_result( myMap.insert_or_assign("a"s, "apple"s     ) );
    print_result( myMap.insert_or_assign("b"s, "banana"s    ) );
    print_result( myMap.insert_or_assign("c"s, "cherry"s    ) );
    print_result( myMap.insert_or_assign("c"s, "clementine"s) );

    for (auto const & node : myMap) {
      print_node(node);
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - emplace"s << '\n';
  {
    std::map<std::string, std::string> mmap;

    // uses pair's move constructor
    mmap.emplace(std::make_pair(std::string("a"s), std::string("a"s)));

    // uses pair's converting move constructor
    mmap.emplace(std::make_pair("b"s, "abcd"s));

    // uses pair's template constructor
    mmap.emplace("d"s, "ddd"s);

    // uses pair's piecewise constructor
    mmap.emplace(std::piecewise_construct,
                 std::forward_as_tuple("c"s),
                 std::forward_as_tuple(10, 'c'));
    // as of C++17, m.try_emplace("c", 10, 'c'); can be used

    for (auto const & kvp : mmap) {
      std::cout << kvp.first << " => "s << kvp.second << '\n';
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - emplace_hint"s << '\n';
  {
    const int nof_operations = 100'500;

    auto map_emplace = []() {
      std::map<int, char> map;
      for(int i_ = 0; i_ < nof_operations; ++i_) {
        map.emplace(i_, 'a');
      }
      return map.size();
    };

    auto map_emplace_hint = []() {
      std::map<int, char> map;
      auto it = map.begin();
      for(int i_ = 0; i_ < nof_operations; ++i_) {
        map.emplace_hint(it, i_, 'b');
        it = map.end();
      }
      return map.size();
    };

    auto map_emplace_hint_wrong = []() {
      std::map<int, char> map;
      auto it = map.begin();
      for(int i_ = nof_operations; i_ > 0; --i_) {
      map.emplace_hint(it, i_, 'c');
        it = map.end();
      }
      return map.size();
    };

    auto map_emplace_hint_corrected = []() {
      std::map<int, char> map;
      auto it = map.begin();
      for(int i_ = nof_operations; i_ > 0; --i_) {
        map.emplace_hint(it, i_, 'd');
        it = map.begin();
      }
      return map.size();
    };

    auto map_emplace_hint_closest = []() {
      std::map<int, char> map;
      auto it = map.begin();
      for(int i_ = 0; i_ < nof_operations; ++i_) {
        it = map.emplace_hint(it, i_, 'e');
      }
      return map.size();
    };

    auto timeit = [](std::function<int()> map_test, std::string what = ""s) {
      auto start = std::chrono::system_clock::now();
      int mapsize = map_test();
      auto stop = std::chrono::system_clock::now();
      std::chrono::duration<double, std::milli> time = stop - start;
      if (what.size() > 0 && mapsize > 0) {
        std::cout << std::fixed << std::setprecision(2) << std::setw(6)
                  << time.count() << "  ms for "s << what << '\n';
      }
    };

    timeit(map_emplace); // stack warmup
    timeit(map_emplace, "plain emplace"s);
    timeit(map_emplace_hint, "emplace with correct hint"s);
    timeit(map_emplace_hint_wrong, "emplace with wrong hint"s);
    timeit(map_emplace_hint_corrected, "corrected emplace"s);
    timeit(map_emplace_hint_closest, "emplace using returned iterator"s);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - try_emplace"s << '\n';
  {
    std::map<std::string, std::string> mmap;

    mmap.try_emplace("a", "a"s);
    mmap.try_emplace("b"s, "abcd");
    mmap.try_emplace("c", 10, 'c');
    mmap.try_emplace("c"s, "Won't be inserted");

    for (auto const & kvp : mmap) {
      std::cout << kvp.first << " => "s << kvp.second << '\n';
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - erase"s << '\n';
  {
    std::map<int, std::string> container = {
      { 1, "one"s  }, { 2, "two"s  }, { 3, "three"s },
      { 4, "four"s }, { 5, "five"s }, { 6, "six"s   },
    };

    // erase all odd numbers from c
    for (auto kvp = container.begin(); kvp != container.end(); ) {
      if (kvp->first % 2 == 1) {
        kvp = container.erase(kvp);
      }
      else {
        ++kvp;
      }
    }

    for (auto & kvp : container) {
      std::cout << kvp.second << ' ';
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - swap"s << '\n';
  {
    using namespace cmap;

    std::map<std::string, std::string>
      m1 {
        { "γ"s, "gamma"s }, { "β"s, "beta"s  },
        { "α"s, "alpha"s }, { "γ"s, "gamma"s },
      },
      m2 {
        { "ε"s, "epsilon"s }, { "δ"s, "delta"s }, { "ε"s, "epsilon"s },
      };

    auto const & ref = *(m1.begin());
    auto const iter = std::next(m1.cbegin());

    std::cout << "──────── before swap ────────\n"s
              << "m1: "s << m1 << "m2: "s << m2 << "ref: "s << ref
              << "\niter: "s << *iter << '\n';

    m1.swap(m2);

    std::cout << "──────── after swap ────────\n"s
              << "m1: "s << m1 << "m2: "s << m2 << "ref: "s << ref
              << "\niter: "s << *iter << '\n';

    // Note that every iterator referring to an element in one container before
    // the swap refers to the same element in the other container after the swap.
    // Same is true for references.

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - extract"s << '\n';
  {
    std::map<int, char> cont{ { 1, 'a' }, { 2, 'b' }, { 3, 'c' } };

    auto print = [](std::pair<int const, char> & ch) {
      std::cout << ' ' << ch.first << '(' << ch.second << ')';
    };

    std::cout << "Start:"s;
    std::for_each(cont.begin(), cont.end(), print);
    std::cout << '\n';

    // Extract node handle and change key
    auto nh = cont.extract(1);
    nh.key() = 4;

    std::cout << "After extract and before insert:"s;
    std::for_each(cont.begin(), cont.end(), print);
    std::cout << '\n';

    // Insert node handle back
    cont.insert(move(nh));

    std::cout << "End:"s;
    std::for_each(cont.begin(), cont.end(), print);
    std::cout << '\n';

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - merge"s << '\n';
  {
    std::map<int, std::string> ma {
      { 1, "apple"s }, { 5, "pear"s }, { 10, "banana"s },
    };

    std::map<int, std::string> mb {
      { 2, "zorro"s }, { 4, "batman"s }, { 5, "X"s }, { 8, "alpaca"s },
    };

    std::map<int, std::string> mu;
    mu.merge(ma);
    std::cout << "ma.size(): "s << ma.size() << '\n';
    mu.merge(mb);
    std::cout << "mb.size(): "s << mb.size() << '\n';
    std::cout << "mb.at(5):  "s << mb.at(5) << '\n';

    for (auto const & kv: mu) {
      std::cout << kv.first << ", " << kv.second << '\n';
    }

    std::cout << '\n';
  }

  /// Lookup
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - count"s << '\n';
  {
    auto mis = std::map<int, std::string> {
      { 1, "one"s }, { 3, "three"s }, { 4, "four"s },
    };

    for (auto nr : { 1, 2, 3, 4, }) {
      std::cout << "key: "s << nr << " count: " << mis.count(nr) << '\n';
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - find"s << '\n';
  {
    using namespace cmapfd;

    // simple comparison demo
    std::map<int, char> example = {
      { 1, 'a' },
      { 2, 'b' },
    };

    auto search = example.find(2);
    if (search != example.end()) {
      std::cout << "Found "s << search->first << " "s << search->second << '\n';
    }
    else {
      std::cout << "Not found\n"s;
    }

    // transparent comparison demo
    std::map<FatKey, char, std::less<>> example2 = {
      { {1, {} }, 'a' },
      { {2, {} }, 'b' },
    };

    LightKey lk = {2};
    auto search2 = example2.find(lk);
    if (search2 != example2.end()) {
      std::cout << "Found "s << search2->first.x << " "s << search2->second << '\n';
    }
    else {
      std::cout << "Not found\n"s;
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - contains"s << '\n';
  {
    std::map<int,char> example = {{1,'a'},{2,'b'}};

    for (int nr : { 2, 5 } ) {
      if (example.contains(nr)) {
        std::cout << nr << ": Found\n"s;
      }
      else {
        std::cout << nr << ": Not found\n"s;
      }
    }

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - equal_range"s << '\n';
  {
    const std::map<int, char const *> mic {
      { 0, "zero" },
      { 1, "one"  },
      { 2, "two"  },
    };

    {
      auto sfp = mic.equal_range(1);
      for (auto & it = sfp.first; it != sfp.second; ++it) {
        std::cout << "mic["s << it->first << "] = "s << it->second << '\n';
      }

      if (sfp.second == mic.find(2)) {
        std::cout << "end of equal_range (sfp.second) is one-past sfp.first\n"s;
      }
      else {
        std::cout << "unexpected; sfp.second expected to be one-past sfp.first\n"s;
      }
    }

    {
      auto sfp = mic.equal_range(-1);
      if (sfp.first == mic.begin()) {
        std::cout << "sfp.first is iterator to first not-less than -1\n"s;
      }
      else {
        std::cout << "unexpected sfp.first\n"s;
      }

      if (sfp.second == mic.begin()) {
        std::cout << "sfp.second is iterator to first element greater-than -1\n"s;
      }
      else {
        std::cout << "unexpected sfp.second\n"s;
      }
    }

    {
      auto sfp = mic.equal_range(3);
      if (sfp.first == mic.end()) {
        std::cout << "sfp.first is iterator to first not-less than 3\n"s;
      }
      else {
        std::cout << "unexpected sfp.first\n"s;
      }

      if (sfp.second == mic.end()) {
        std::cout << "sfp.second is iterator to first element greater-than 3\n"s;
      }
      else {
        std::cout << "unexpected sfp.second\n"s;
      }
    }

    std::cout << '\n';
  }

  /// Observers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - key_comp, value_comp"s << '\n';
  {
    // Example module 97 key compare function
    struct ModCmp {
      bool operator()(int const lhs, int const rhs) const {
        return (lhs % 97) < (rhs % 97);
      }
    };

    std::map<int, char, ModCmp> cont;
    cont = {
      { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' }, { 5, 'e' },
    };

    std::cout << "key_comp"s << '\n';
    {
      auto comp_func = cont.key_comp();

      auto constexpr val = 100;

      for (auto it : cont) {
        bool before = comp_func(it.first, val);
        bool after  = comp_func(val, it.first);

        std::cout << '(' << it.first << ',' << it.second;
        if (!before && !after) {
          std::cout << ") equivalent to key "s << val << '\n';
        }
        else if (before) {
          std::cout << ") goes before key "s <<val << '\n';
        }
        else if (after) {
          std::cout << ") goes after key "s << val << '\n';
        }
        else {
          assert(0); // Cannot happen
        }
      }
    }

    std::cout << "value_comp"s << '\n';
    {
      auto comp_func = cont.value_comp();

      std::pair<int, char> constexpr val = { 100, 'a' };

      for (auto it : cont) {
        bool before = comp_func(it, val);
        bool after = comp_func(val, it);

        std::cout << '(' << it.first << ',' << it.second;
        if (!before && !after) {
          std::cout << ") equivalent to key " << val.first << '\n';
        }
        else if (before) {
          std::cout << ") goes before key " << val.first << '\n';
        }
        else if (after) {
          std::cout << ") goes after key " << val.first << '\n';
        }
        else {
          assert(0); // Cannot happen
        }
      }
    }

    std::cout << '\n';
  }

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - std::operator==, etc."s << '\n';
  {
    std::map<int, char> alice { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, };
    std::map<int, char> bob   { { 7, 'Z' }, { 8, 'Y' }, { 9, 'X' }, { 10, 'W' }, };
    std::map<int, char> eve   { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, };

    std::cout << std::boolalpha;

    // Compare non equal containers
    std::cout << "alice == bob returns " << (alice == bob) << '\n';
    std::cout << "alice != bob returns " << (alice != bob) << '\n';
    std::cout << "alice <  bob returns " << (alice < bob) << '\n';
    std::cout << "alice <= bob returns " << (alice <= bob) << '\n';
    std::cout << "alice >  bob returns " << (alice > bob) << '\n';
    std::cout << "alice >= bob returns " << (alice >= bob) << '\n';

    std::cout << '\n';

    // Compare equal containers
    std::cout << "alice == eve returns " << (alice == eve) << '\n';
    std::cout << "alice != eve returns " << (alice != eve) << '\n';
    std::cout << "alice <  eve returns " << (alice < eve) << '\n';
    std::cout << "alice <= eve returns " << (alice <= eve) << '\n';
    std::cout << "alice >  eve returns " << (alice > eve) << '\n';
    std::cout << "alice >= eve returns " << (alice >= eve) << '\n';

    std::cout << '\n';

#if (__cplusplus > 201707L)
    //  TODO:  add spaceship operator tests
    auto eq = std::is_eq  (alice <=> eve);
    auto ne = std::is_neq (alice <=> eve);
    auto lt = std::is_lt  (alice <=> eve);
    auto le = std::is_lteq(alice <=> eve);
    auto gt = std::is_gt  (alice <=> eve);
    auto ge = std::is_gteq(alice <=> eve);
#endif  /* (__cplusplus > 201707L) */

    std::cout << std::boolalpha;

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - std::swap"s << '\n';
  {
    std::map<int, char> alice { { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, };
    std::map<int, char> bob   { { 7, 'Z' }, { 8, 'Y' }, { 9, 'X' }, { 10, 'W' }, };

    auto print = [](std::pair<int const, char> & nr) {
      std::cout << ' ' << nr.first << '(' << nr.second << ')';
    };

    // Print state before swap
    std::cout << "alice:"s;
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << '\n'
              << "bob  :"s;
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';

    std::cout << "-- SWAP\n"s;
    std::swap(alice, bob);

    // Print state after swap
    std::cout << "alice:"s;
    std::for_each(alice.begin(), alice.end(), print);
    std::cout << '\n'
              << "bob  :"s;
    std::for_each(bob.begin(), bob.end(), print);
    std::cout << '\n';

    std::cout << '\n';
  }

#if (__cplusplus > 201707L)
  //`TODO:  add C++20 feature tests
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::map - std::erase_if"s << '\n';
  {
    using namespace cmap;

    std::map<int, char> data {
      { 1, 'a' }, { 2, 'b' }, { 3, 'c' }, { 4, 'd' },
      { 5, 'e' }, { 4, 'f' }, { 5, 'g' }, { 5, 'g' },
    };

    std::cout << "Original:\n" << data << '\n';

    auto const count = std::erase_if(data, [](auto const & item) {
      auto const & [key, value] = item;
      return (key & 1) == 1;
    });

    std::cout << "Erase items with odd keys:\n"s << data << '\n'
            << count << " items removed.\n"s;

    std::cout << '\n';
  }
#endif  /* (__cplusplus > 201707L) */

  std::cout << std::endl; //  make sure cout is flushed.

  return 0;
}

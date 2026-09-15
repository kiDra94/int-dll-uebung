#include "int_dll.hpp"

#include <catch2/catch_test_macros.hpp>
#include <sstream>


TEST_CASE("constructors", "[constructors]") {
  ds::IntDoublyLinkedList list;

  SECTION("empty_list") {
    CHECK(list.head() == nullptr);
    CHECK(list.tail() == nullptr);
    CHECK(list.size() == 0);
    CHECK(list.empty());
  }
}


TEST_CASE("push_elements", "[push]") {
  ds::IntDoublyLinkedList list;

  SECTION("push_front_into_empty_list") {
    list.push_front(10);
    CHECK(list.head() == list.tail());
    CHECK(list.head()->data == 10);
    CHECK(list.head()->next == nullptr);
    CHECK(list.head()->prev == nullptr);
    CHECK(list.size() == 1);
    CHECK(not list.empty());
  }

  SECTION("push_back_into_empty_list") {
    list.push_back(10);
    CHECK(list.head() == list.tail());
    CHECK(list.tail()->data == 10);
    CHECK(list.tail()->next == nullptr);
    CHECK(list.tail()->prev == nullptr);
    CHECK(list.size() == 1);
  }

  SECTION("push_front_links_both_directions") {
    list.push_front(10);
    list.push_front(5);
    CHECK(list.head()->data == 5);
    CHECK(list.tail()->data == 10);
    CHECK(list.head()->next == list.tail());
    CHECK(list.tail()->prev == list.head());
    CHECK(list.head()->prev == nullptr);
    CHECK(list.tail()->next == nullptr);
  }

  SECTION("push_back_links_both_directions") {
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    CHECK(list.head()->data == 10);
    CHECK(list.head()->next->data == 20);
    CHECK(list.head()->next->next->data == 30);
    CHECK(list.tail()->data == 30);
    CHECK(list.tail()->prev->data == 20);
    CHECK(list.tail()->prev->prev == list.head());
    CHECK(list.size() == 3);
  }

  SECTION("push_front_and_push_back_mixed") {
    list.push_back(2);
    list.push_front(1);
    list.push_back(3);
    list.push_front(0);
    CHECK(list.size() == 4);
    CHECK(list.head()->data == 0);
    CHECK(list.head()->next->data == 1);
    CHECK(list.tail()->prev->data == 2);
    CHECK(list.tail()->data == 3);
    CHECK(list.head()->next->next == list.tail()->prev);
  }
}


TEST_CASE("element_access", "[access]") {
  ds::IntDoublyLinkedList list;

  SECTION("access_data_in_the_front_and_back") {
    list.push_back(10);
    CHECK(list.front() == 10);
    CHECK(list.back() == 10);
    list.push_back(11);
    CHECK(list.front() == 10);
    CHECK(list.back() == 11);
    list.push_front(3);
    CHECK(list.front() == 3);
    CHECK(list.back() == 11);
  }

  SECTION("random_access") {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    CHECK(list.at(0) == 1);
    CHECK(list.at(1) == 2);
    CHECK(list.at(2) == 3);
    CHECK(list.at(3) == 4);
    CHECK(list.at(4) == 5);
  }

  SECTION("empty_list_throws") {
    CHECK_THROWS(list.front());
    CHECK_THROWS(list.back());
    CHECK_THROWS(list.at(0));
  }

  SECTION("at_throws_only_if_out_of_bounds") {
    list.push_back(1);
    list.push_back(2);
    CHECK_NOTHROW(list.at(0));
    CHECK_NOTHROW(list.at(1));
    CHECK_THROWS(list.at(2));
    CHECK_THROWS(list.at(100));
  }
}


TEST_CASE("pop_elements", "[pop]") {
  ds::IntDoublyLinkedList list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  SECTION("pop_front_returns_first_element") {
    CHECK(list.pop_front() == 10);
    CHECK(list.size() == 2);
    CHECK(list.front() == 20);
    CHECK(list.head()->prev == nullptr);
    CHECK(list.head()->next == list.tail());
  }

  SECTION("pop_back_returns_last_element") {
    CHECK(list.pop_back() == 30);
    CHECK(list.size() == 2);
    CHECK(list.back() == 20);
    CHECK(list.tail()->next == nullptr);
    CHECK(list.tail()->prev == list.head());
  }

  SECTION("pop_until_empty") {
    CHECK(list.pop_back() == 30);
    CHECK(list.pop_front() == 10);
    CHECK(list.pop_back() == 20);
    CHECK(list.empty());
    CHECK(list.size() == 0);
    CHECK(list.head() == nullptr);
    CHECK(list.tail() == nullptr);
  }

  SECTION("list_is_usable_after_being_emptied") {
    list.pop_front();
    list.pop_front();
    list.pop_front();
    list.push_back(1);
    CHECK(list.head() == list.tail());
    CHECK(list.front() == 1);
    CHECK(list.back() == 1);
  }

  SECTION("pop_from_empty_list_throws") {
    ds::IntDoublyLinkedList empty;
    CHECK_THROWS(empty.pop_front());
    CHECK_THROWS(empty.pop_back());
  }
}


TEST_CASE("insert_at_position", "[insert]") {
  ds::IntDoublyLinkedList list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  SECTION("insert_at_front") {
    list.insert(0, 5);
    CHECK(list.size() == 4);
    CHECK(list.head()->data == 5);
    CHECK(list.head()->prev == nullptr);
    CHECK(list.head()->next->data == 10);
    CHECK(list.head()->next->prev == list.head());
  }

  SECTION("insert_in_the_middle") {
    list.insert(2, 25);
    CHECK(list.size() == 4);
    CHECK(list.at(0) == 10);
    CHECK(list.at(1) == 20);
    CHECK(list.at(2) == 25);
    CHECK(list.at(3) == 30);
    CHECK(list.head()->next->next->data == 25);
    CHECK(list.tail()->prev->data == 25);
    CHECK(list.tail()->prev->prev->data == 20);
  }

  SECTION("insert_at_back") {
    list.insert(3, 40);
    CHECK(list.size() == 4);
    CHECK(list.tail()->data == 40);
    CHECK(list.tail()->next == nullptr);
    CHECK(list.tail()->prev->data == 30);
  }

  SECTION("insert_into_empty_list") {
    ds::IntDoublyLinkedList empty;
    empty.insert(0, 7);
    CHECK(empty.size() == 1);
    CHECK(empty.head() == empty.tail());
    CHECK(empty.front() == 7);
  }

  SECTION("insert_out_of_bounds_throws") {
    CHECK_THROWS(list.insert(4, 1));
    CHECK_THROWS(list.insert(100, 1));
    CHECK(list.size() == 3);
  }
}


TEST_CASE("remove_by_value", "[remove]") {
  ds::IntDoublyLinkedList list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(20);

  SECTION("remove_middle_element") {
    CHECK(list.remove(10));
    CHECK(list.size() == 2);
    CHECK(list.head()->data == 5);
    CHECK(list.head()->next == list.tail());
    CHECK(list.tail()->prev == list.head());
  }

  SECTION("remove_first_element") {
    CHECK(list.remove(5));
    CHECK(list.head()->data == 10);
    CHECK(list.head()->prev == nullptr);
  }

  SECTION("remove_last_element") {
    CHECK(list.remove(20));
    CHECK(list.tail()->data == 10);
    CHECK(list.tail()->next == nullptr);
  }

  SECTION("remove_all_elements") {
    CHECK(list.remove(10));
    CHECK(list.remove(20));
    CHECK(list.remove(5));
    CHECK(list.empty());
    CHECK(list.head() == nullptr);
    CHECK(list.tail() == nullptr);
  }

  SECTION("remove_node_that_does_not_exist") {
    CHECK(not list.remove(123));
    CHECK(not list.remove(0));
    CHECK(not list.remove(-5));
    CHECK(list.size() == 3);
  }

  SECTION("remove_from_empty_list") {
    ds::IntDoublyLinkedList empty;
    CHECK(not empty.remove(10));
    CHECK(empty.size() == 0);
  }

  SECTION("remove_first_matching_node") {
    list.push_back(10);
    CHECK(list.remove(10));
    CHECK(list.size() == 3);
    CHECK(list.at(0) == 5);
    CHECK(list.at(1) == 20);
    CHECK(list.at(2) == 10);
    CHECK(list.remove(10));
    CHECK(not list.remove(10));
    CHECK(list.size() == 2);
  }
}


TEST_CASE("count_occurrences", "[count]") {
  ds::IntDoublyLinkedList list;

  SECTION("count_number_of_occurrences_in_empty_list") {
    CHECK(list.count(1) == 0);
    CHECK(list.count(0) == 0);
  }

  SECTION("count_number_of_occurrences") {
    list.push_back(10);
    list.push_back(10);
    list.push_back(5);
    list.push_front(20);
    list.push_front(20);
    list.push_front(20);
    CHECK(list.count(10) == 2);
    CHECK(list.count(5) == 1);
    CHECK(list.count(20) == 3);
    CHECK(list.count(0) == 0);
    CHECK(list.count(-3) == 0);
  }
}


TEST_CASE("reverse_the_order_of_the_elements", "[reverse]") {
  ds::IntDoublyLinkedList list;

  SECTION("reverse_empty_list") {
    list.reverse();
    CHECK(list.empty());
    CHECK(list.head() == nullptr);
    CHECK(list.tail() == nullptr);
  }

  SECTION("reverse_single_element") {
    list.push_back(1);
    list.reverse();
    CHECK(list.head() == list.tail());
    CHECK(list.front() == 1);
  }

  SECTION("reverse_multiple_elements") {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    const auto* old_head = list.head();
    list.reverse();
    CHECK(list.tail() == old_head);  // no new nodes were created
    CHECK(list.head()->data == 4);
    CHECK(list.head()->next->data == 3);
    CHECK(list.head()->next->next->data == 2);
    CHECK(list.tail()->data == 1);
    CHECK(list.tail()->prev->data == 2);
    CHECK(list.tail()->prev->prev->data == 3);
    CHECK(list.head()->prev == nullptr);
    CHECK(list.tail()->next == nullptr);
    CHECK(list.size() == 4);
  }

  SECTION("list_is_usable_after_reverse") {
    list.push_back(1);
    list.push_back(2);
    list.reverse();
    list.push_back(0);
    list.push_front(3);
    CHECK(list.pop_back() == 0);
    CHECK(list.pop_back() == 1);
    CHECK(list.pop_front() == 3);
    CHECK(list.pop_front() == 2);
    CHECK(list.empty());
  }
}


TEST_CASE("copy", "[copy]") {
  ds::IntDoublyLinkedList list;
  list.push_back(10);
  list.push_back(5);
  list.push_back(20);

  SECTION("copy_constructor") {
    ds::IntDoublyLinkedList other(list);
    CHECK(other.size() == 3);
    CHECK(other.head() != list.head());
    CHECK(other.tail() != list.tail());
    CHECK(other.at(0) == 10);
    CHECK(other.at(1) == 5);
    CHECK(other.at(2) == 20);
    CHECK(other.tail()->prev->data == 5);

    other.push_back(1);
    list.push_front(1);
    CHECK(other.size() == 4);
    CHECK(list.size() == 4);
    CHECK(other.front() == 10);
    CHECK(other.back() == 1);
    CHECK(list.front() == 1);
    CHECK(list.back() == 20);
  }

  SECTION("copy_constructor_of_empty_list") {
    ds::IntDoublyLinkedList empty;
    ds::IntDoublyLinkedList other(empty);
    CHECK(other.empty());
    CHECK(other.head() == nullptr);
    CHECK(other.tail() == nullptr);
  }

  SECTION("copy_assignment") {
    ds::IntDoublyLinkedList other;
    other.push_back(99);
    other = list;
    CHECK(other.size() == 3);
    CHECK(other.head() != list.head());
    CHECK(other.front() == 10);
    CHECK(other.back() == 20);
    CHECK(other.count(99) == 0);

    other.pop_front();
    CHECK(list.size() == 3);
    CHECK(list.front() == 10);
  }

  SECTION("copy_assignment_of_empty_list") {
    ds::IntDoublyLinkedList empty;
    list = empty;
    CHECK(list.empty());
    CHECK(list.head() == nullptr);
    CHECK(list.tail() == nullptr);
  }

  SECTION("self_assignment") {
    ds::IntDoublyLinkedList& same = list;
    list = same;
    CHECK(list.size() == 3);
    CHECK(list.front() == 10);
    CHECK(list.back() == 20);
  }

  SECTION("chained_assignment") {
    ds::IntDoublyLinkedList a;
    ds::IntDoublyLinkedList b;
    a = b = list;
    CHECK(a.size() == 3);
    CHECK(b.size() == 3);
    CHECK(a.head() != b.head());
  }
}


TEST_CASE("non_member_functions", "[ostream]") {
  ds::IntDoublyLinkedList list;

  SECTION("print_list") {
    list.push_back(10);
    list.push_back(5);
    list.push_back(20);
    std::stringstream ss;
    ss << list;
    CHECK(ss.str() == "dll[10, 5, 20]");
  }

  SECTION("print_empty_list") {
    std::stringstream ss;
    ss << list;
    CHECK(ss.str() == "dll[]");
  }

  SECTION("operator_returns_the_stream") {
    list.push_back(-1);
    std::stringstream ss;
    ss << list << " " << list;
    CHECK(ss.str() == "dll[-1] dll[-1]");
  }
}


TEST_CASE("const_correctness", "[const]") {
  ds::IntDoublyLinkedList list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(2);
  const ds::IntDoublyLinkedList& c = list;

  SECTION("read_only_member_functions_are_const") {
    CHECK(c.head()->data == 1);
    CHECK(c.tail()->data == 2);
    CHECK(c.front() == 1);
    CHECK(c.back() == 2);
    CHECK(c.size() == 3);
    CHECK(not c.empty());
    CHECK(c.at(1) == 2);
    CHECK(c.count(2) == 2);
  }

  SECTION("copy_from_const_list") {
    ds::IntDoublyLinkedList other(c);
    CHECK(other.size() == 3);
  }
}


TEST_CASE("performance", "[performance]") {
  ds::IntDoublyLinkedList list;
  const int64_t n = 1000000;

  SECTION("add_and_remove_1,000,000_elements_at_the_back") {
    for (int64_t i(0); i < n; ++i) {
      list.push_back(i);
    }
    CHECK(list.size() == 1000000);
    for (int64_t i(0); i < n; ++i) {
      list.pop_back();
    }
    CHECK(list.empty());
  }

  SECTION("add_and_remove_1,000,000_elements_at_the_front") {
    for (int64_t i(0); i < n; ++i) {
      list.push_front(i);
    }
    CHECK(list.size() == 1000000);
    CHECK(list.back() == 0);
    for (int64_t i(0); i < n; ++i) {
      list.pop_front();
    }
    CHECK(list.empty());
  }

  SECTION("destroy_a_list_with_1,000,000_elements") {
    for (int64_t i(0); i < n; ++i) {
      list.push_back(i);
    }
    CHECK(list.back() == n - 1);
  }
}

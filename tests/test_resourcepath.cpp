/*
 * test_resourcepath.cc
 *
 *  Created on: 23.08.2020
 *      Author: blox
 */

#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "resourcepath.hpp"

BOOST_AUTO_TEST_CASE(construct_path_valid)
{
  TC::REST::ResourcePath empty1, empty2("/"), empty3("");
  BOOST_CHECK_EQUAL(empty1, empty2);
  BOOST_CHECK_EQUAL(empty2, empty3);
  BOOST_CHECK_EQUAL(empty1.GetSize(), 0);

  {
    TC::REST::ResourcePath path("/A");
    BOOST_CHECK_EQUAL(path.GetSize(), 1);
    BOOST_CHECK_EQUAL(path[0], "A");
  }

  {
    TC::REST::ResourcePath path("/abc/def");
    BOOST_CHECK_EQUAL(path.GetSize(), 2);
    BOOST_CHECK_EQUAL(path[0], "abc");
    BOOST_CHECK_EQUAL(path[1], "def");

    TC::REST::ResourcePath path2("/abc/def/");
    BOOST_CHECK_EQUAL(path2.GetSize(), 2);
    BOOST_CHECK_EQUAL(path2[0], "abc");
    BOOST_CHECK_EQUAL(path2[1], "def");

    BOOST_CHECK_EQUAL(path, path2);

    BOOST_CHECK_EQUAL(path2.ToString(), "/abc/def");
  }
}

BOOST_AUTO_TEST_CASE(contruct_path_invalid)
{
  BOOST_CHECK_THROW(TC::REST::ResourcePath("X"),             std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/a//b/c"),       std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/de%f"),     std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/def%"),     std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/def%00"),   std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/def%00/b"), std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/d ef"),     std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/d:ef"),     std::invalid_argument);
  BOOST_CHECK_THROW(TC::REST::ResourcePath("/abc/def\n"),    std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(path_encoding_valid)
{
  {
    TC::REST::ResourcePath path("/abc/this%20is%20a%20sentence");
    BOOST_CHECK_EQUAL(path.GetSize(), 2);
    BOOST_CHECK_EQUAL(path[0], "abc");
    BOOST_CHECK_EQUAL(path[1], "this is a sentence");
    BOOST_CHECK_EQUAL(path.ToString(), "/abc/this%20is%20a%20sentence");
  }
  {
    TC::REST::ResourcePath path("/abc/a%3Ab%2fc%3Fd%23e/f%5Bg%5dh%40i%21");
    BOOST_CHECK_EQUAL(path.GetSize(), 3);
    BOOST_CHECK_EQUAL(path[0], "abc");
    BOOST_CHECK_EQUAL(path[1], "a:b/c?d#e");
    BOOST_CHECK_EQUAL(path[2], "f[g]h@i!");

    path.AddComponent("; ,");
    BOOST_CHECK_EQUAL(path.ToString(), "/abc/a%3Ab%2Fc%3Fd%23e/f%5Bg%5Dh%40i%21/%3B%20%2C");
  }
}

BOOST_AUTO_TEST_CASE(path_comparison)
{
  {
    TC::REST::ResourcePath layer1("/abc");
    TC::REST::ResourcePath layer2("/abc/def"), layer2_clone(layer2);
    TC::REST::ResourcePath layer2_diff("/abc/d3f");
    TC::REST::ResourcePath layer3("/abc/def/ghi");
    TC::REST::ResourcePath layer4("/abc/def/ghi/jkl");

    BOOST_CHECK_EQUAL(layer1.IsParent(layer2), true);
    BOOST_CHECK_EQUAL(layer1.IsParent(layer3), true);
    BOOST_CHECK_EQUAL(layer3.IsChild(layer2),  true);
    BOOST_CHECK_EQUAL(layer3.IsChild(layer1),  true);

    BOOST_CHECK_EQUAL(layer1.IsParent(layer2, TC::REST::ResourcePath::CP_IMMEDIATE), true);
    BOOST_CHECK_EQUAL(layer1.IsParent(layer3, TC::REST::ResourcePath::CP_IMMEDIATE), false);
    BOOST_CHECK_EQUAL(layer3.IsChild(layer2,  TC::REST::ResourcePath::CP_IMMEDIATE), true);
    BOOST_CHECK_EQUAL(layer3.IsChild(layer1,  TC::REST::ResourcePath::CP_IMMEDIATE), false);

    BOOST_CHECK_EQUAL(layer2.IsChild(layer2_clone), false);
    BOOST_CHECK_EQUAL(layer2.IsParent(layer2_clone), false);

    BOOST_CHECK_EQUAL(layer3.IsChild(layer2_diff), false);
    BOOST_CHECK_EQUAL(layer2_diff.IsParent(layer4), false);
  }
}

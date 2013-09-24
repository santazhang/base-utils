#pragma once

#include <vector>
#include <iostream>

namespace b0 {

class TestCase {
    const char* group_;
    const char* name_;
    int failures_;
public:
    TestCase(const char* group, const char* name): group_(group), name_(name), failures_(0) { }
    virtual void run() = 0;
    const char* group() { return group_; }
    const char* name() { return name_; }
    void reset() { failures_ = 0; }
    void fail();
    int failures() { return failures_; }
};

// singleton
class TestMgr {
    TestMgr() { }
    static TestMgr* instance_s;
    std::vector<TestCase*> tests_;
public:
    static TestMgr* instance();
    TestCase* reg(TestCase*);
    void run(int argc, char* argv[]);
};

} // namespace b0

#define TEST_CLASS_NAME(group, name) \
    TestCase_ ## group ## _ ## name

#define TEST(group, name) \
    class TEST_CLASS_NAME(group, name) : public ::b0::TestCase { \
        static TestCase* me_s; \
    public: \
        TEST_CLASS_NAME(group, name)(); \
        void run(); \
    }; \
    TEST_CLASS_NAME(group, name)::TEST_CLASS_NAME(group, name)(): \
        TestCase(#group, #name) { } \
    ::b0::TestCase* TEST_CLASS_NAME(group, name)::me_s = \
        ::b0::TestMgr::instance()->reg(new TEST_CLASS_NAME(group, name)); \
    void TEST_CLASS_NAME(group, name)::run()

#define RUN_TESTS(argc, argv) ::b0::TestMgr::instance()->run(argc, argv);

#define EXPECT_TRUE(a) \
    { \
        auto va = (a); \
        if (!va) { \
            fail(); \
            std::cout << "    *** expected true: '" << #a << "', got false (" \
                << __FILE__ << ':' << __LINE__  << ')' << std::endl; \
        } \
    }

#define EXPECT_FALSE(a) \
    { \
        auto va = (a); \
        if (va) { \
            fail(); \
            std::cout << "    *** expected false: '" << #a << "', got true (" \
                << __FILE__ << ':' << __LINE__  << ')' << std::endl; \
        } \
    }

#define EXPECT_EQ(a, b) \
    { \
        auto va = (a); \
        auto vb = (b); \
        if (va != vb) { \
            fail(); \
            std::cout << "    *** expected equal: '" << #a << " == " << #b << "', got " << va << " != " << vb << " (" \
                << __FILE__ << ':' << __LINE__  << ')' << std::endl; \
        } \
    }

#define EXPECT_NEQ(a, b) \
    { \
        auto va = (a); \
        auto vb = (b); \
        if (va == vb) { \
            fail(); \
            std::cout << "    *** expected not equal: '" << #a << " != " << #b << "', got " << va << " == " << vb << " (" \
                << __FILE__ << ':' << __LINE__  << ')' << std::endl; \
        } \
    }



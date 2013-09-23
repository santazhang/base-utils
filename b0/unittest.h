#pragma once

#include <vector>

namespace b0 {

class TestCase {
    const char* group_;
    const char* name_;
    bool failed_;
public:
    TestCase(const char* group, const char* name): group_(group), name_(name) { }
    virtual void run() = 0;
    const char* group() { return group_; }
    const char* name() { return name_; }
    void set_failed(bool failed) { failed_ = failed; }
    bool failed() { return failed_; }
};

// singleton
class TestMgr {
    TestMgr();
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
            set_failed(true); \
            Log_error("expected true: %s, got false", #a); \
        } \
    }

#define EXPECT_FALSE(a) \
    { \
        auto va = (a); \
        if (va) { \
            set_failed(true); \
            Log_error("expected false: %s, got true", #a); \
        } \
    }

#define EXPECT_EQ(a, b) \
    { \
        auto va = (a); \
        auto vb = (b); \
        if (va != vb) { \
            set_failed(true); \
            Log_error("expected equal: %s == %s", #a, #b); \
        } \
    }

#define EXPECT_NEQ(a, b) \
    { \
        auto va = (a); \
        auto vb = (b); \
        if (va == vb) { \
            set_failed(true); \
            Log_error("expected not equal: %s != %s", #a, #b); \
        } \
    }



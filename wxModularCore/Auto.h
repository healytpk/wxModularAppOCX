#pragma once

template<class L>
class AtScopeExit {
    L &m_lambda;
public:
    AtScopeExit(L &action) : m_lambda(action) {}
    ~AtScopeExit(void) noexcept
    {
        try { m_lambda(); } catch(...){}
    }
};

#define TOKEN_PASTEx(x, y) x ## y
#define TOKEN_PASTE(x, y) TOKEN_PASTEx(x, y)

#define Auto_INTERNAL1(lname, aname, ...) \
    auto lname = [&]() { __VA_ARGS__; }; \
    AtScopeExit<decltype(lname)> aname(lname);

#define Auto_INTERNAL2(ctr, ...) \
    Auto_INTERNAL1(TOKEN_PASTE(Auto_func_, ctr), \
        TOKEN_PASTE(Auto_instance_, ctr), __VA_ARGS__)

#define Auto(...) \
    Auto_INTERNAL2(__COUNTER__, __VA_ARGS__)

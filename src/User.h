#include <Wt/Dbo/Dbo>
#include <Wt/WGlobal>
#if !defined(_user_h_included_)
#define _user_h_included_

namespace dbo = Wt::Dbo;

class User;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;

class User {
public:
    template<class Action>
    void persist(Action& a)
    {
    }
};

DBO_EXTERN_TEMPLATES(User)


#endif

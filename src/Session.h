#ifndef SESSION_H_
#define SESSION_H_

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Auth/Login>
#include "User.h"


typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

class Session : public Wt::Dbo::Session
{

private:
    Wt::Dbo::backend::Sqlite3 connection;
    UserDatabase *users;
    Wt::Auth::Login login;

public:
    Session(const std::string& sqliteDb);
    static void configureAuth();
    Wt::Auth::AbstractUserDatabase& getUsers();
    Wt::Auth::Login& getLogin() { return login; }
    ~Session();
    
    static const Wt::Auth::AuthService& getBaseAuth();
    static const Wt::Auth::PasswordService& getPasswordAuth();
    static const std::vector<const Wt::Auth::OAuthService *>& getOAuthServices();

};




#endif //SESSION_H_

#include "Session.h"

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"
#include "Wt/Auth/GoogleService"
#include "Wt/Auth/FacebookService"
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

//===============================================================

namespace {
    class MyOAuth : public std::vector<const Wt::Auth::OAuthService *>
    {
    public:
        ~MyOAuth()
        {
            for (unsigned i = 0; i < size(); ++i)
                delete (*this)[i];
        }
    };

    Wt::Auth::AuthService myAuthService;
    Wt::Auth::PasswordService myPasswordService(myAuthService);
    std::vector<const Wt::Auth::OAuthService *> myOAuth;
    MyOAuth myOAuthServices;
}

//========================================================================

void Session::configureAuth()
{
    myAuthService.setAuthTokensEnabled(true, "logincookie");
    myAuthService.setEmailVerificationEnabled(true);
    //myAuthService.setEmailVerificationRequired(true);

    Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
    verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
    myPasswordService.setVerifier(verifier);
    myPasswordService.setAttemptThrottlingEnabled(true);
//     myPasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

    if (Wt::Auth::GoogleService::configured())
        myOAuthServices.push_back(new Wt::Auth::GoogleService(myAuthService));

    if (Wt::Auth::FacebookService::configured())
        myOAuthServices.push_back(new Wt::Auth::FacebookService(myAuthService));

    for (unsigned i = 0; i < myOAuthServices.size(); ++i)
        myOAuthServices[i]->generateRedirectEndpoint();
}


const Wt::Auth::AuthService& Session::getBaseAuth()
{
    return myAuthService;
}

const Wt::Auth::PasswordService& Session::getPasswordAuth()
{
    return myPasswordService;
}

const std::vector<const Wt::Auth::OAuthService *>& Session::getOAuthServices()
{
    return myOAuth;
}

//========================================================================

Session::Session(const std::string& sqliteDb)
        : connection(sqliteDb)
{

    setConnection(connection);
    connection.setProperty("show-queries", "true");

    mapClass<User>("user");
    mapClass<AuthInfo>("auth_info");
    mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    mapClass<AuthInfo::AuthTokenType>("auth_token");

    try {
        createTables();
        std::cerr << "Created database." << std::endl;
    } catch (Wt::Dbo::Exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Using existing database";
    }

    users = new HsUserDatabase(*this);
}

//===============================================================

void Session::registerUser(const std::string& name, const std::string& password)
{
    dbo::Transaction transaction(*const_cast<Session*>(this));
    Wt::Auth::User authUser = users->registerNew();
    authUser.addIdentity(Wt::Auth::Identity::LoginName, name);
    getPasswordAuth().updatePassword(authUser, password);

    dbo::ptr<User> user = add(new User());

    dbo::ptr<AuthInfo> authInfo = users->find(authUser);
    authInfo.modify()->setUser(user);
}

//===============================================================

Session::~Session()
{
    delete users;
}

HsUserDatabase& Session::getUsers()
{
    return *users;
}

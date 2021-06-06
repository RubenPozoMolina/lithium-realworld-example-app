
#include <lithium_http_server.hh>
#include <lithium_pgsql.hh>

#include "symbols.hh"

int main() {

  using namespace li;

  // The postgres database that we use to store the users.
  auto db = pgsql_database(s::host = "localhost", s::database = "postgres", s::user = "postgres",
                          s::password = "lithium_test", s::port = 5432, s::charset = "utf8");

  // We declare the users ORM.
  //   Per user, we need a unique id, login and a password.
  auto users = sql_orm_schema(db, "users")
                   .fields(s::id(s::auto_increment, s::primary_key) = int(),
                           s::login = std::string(), s::password = std::string(),
                           s::email = std::string());
  users.connect().create_table_if_not_exists();

  // The session will be place in an in memory hashmap.
  // They store the user_id of the logged used.
  auto sessions = hashmap_http_session("test_cookie", s::user_id = -1);

  // This authentication module implements the login, logout, signup and current_user logic.
  // We specify here that we use the login and password field of the user table
  // to authenticate the users.
  // This means that the client will have to pass a login and password POST parameters
  // to the login HTTP route.
  // For the signup, the module will ask for every fields (login, password) except the auto
  // increment field (i.e. the id field).
  auto auth = http_authentication(sessions, users, s::login, s::password);

  auto articles = sql_orm_schema(db, "articles")
                   .fields(s::id(s::auto_increment, s::primary_key) = int(),
                           // We mark the user_id as computed so the CRUD api does not
                           // require it in the create and update methods.
                           s::user_id(s::computed) = int(), 
                           s::title = std::string(),
                           s::description = std::string(),
                           s::body = std::string())

                   .callbacks(
                       s::validate =
                           [&](auto article, http_request&, http_response&) {
                             if (article.title.size() > 0 and article.body.size() > 0)
                               throw http_error::bad_request("Cannot post an empty article.");
                           },

                       s::before_insert =
                           [&](auto& article, http_request& req, http_response& resp) {
                             auto u = auth.current_user(req, resp);
                             if (!u)
                               throw http_error::unauthorized("Please login to create an article.");
                             article.user_id = u->id;
                           },

                       s::before_update =
                           [&](auto& article, http_request& req, http_response& resp) {
                             auto u = auth.current_user(req, resp);
                             if (!u || u->id != article.user_id)
                               throw http_error::unauthorized("This article does not belong to you.");
                           });
  articles.connect().create_table_if_not_exists();                          

  http_api conduit_api;
  
  conduit_api.add_subapi("/api/users", http_authentication_api(auth));
  conduit_api.get("/api/articles") = [&](http_request& request, http_response& response) {
    response.write("articles");
  };

  http_serve(conduit_api, 12345);
}

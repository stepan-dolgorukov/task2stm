#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string_view>

void show_how_to_use(void) {
  std::cout << "./client --command <command>" << '\n';
  std::cout << R"(Доступные команды:
* 11 — зажечь LD1
* 10 — потушить LD1
* 21 — зажечь LD2
* 20 — потушить LD2
* 31 — зажечь LD3
* 30 — потушить LD3)"
            << '\n';
}

int
main(int nargs, char* args[]) {
  using namespace boost;
  namespace prop = program_options;

  prop::options_description desc{};
  desc.add_options()
    ("help", "Как пользоваться программой")
    ("command", prop::value<std::string>(), "Команда серверу")
  ;

  prop::variables_map varmap;
  prop::store(prop::parse_command_line(nargs, args, desc), varmap);
  prop::notify(varmap);

  if (varmap.count("help")) {
    show_how_to_use();
    return 0;
  }

  if (!varmap.count("command")) {
    show_how_to_use();
    return 0;
  }

  const std::string_view command{varmap["command"].as<std::string>()};
  asio::io_context context{};

  const auto server_addr{asio::ip::make_address("192.168.0.10")};
  asio::ip::udp::endpoint endpoint{server_addr, 3333u};
  asio::ip::udp::socket socket{context};

  socket.connect(endpoint);

  auto send_buf{asio::buffer(command)};
  socket.send(send_buf);
}

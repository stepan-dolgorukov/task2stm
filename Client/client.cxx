#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string_view>
#include <exception>

void
show_available_commands(void) {
  std::cout << "Доступные команды:" << '\n'
            << "* 11 — зажечь LD1" << '\n'
            << "* 10 — потушить LD1" << '\n'
            << "* 21 — зажечь LD2" << '\n'
            << "* 20 — потушить LD2" << '\n'
            << "* 31 — зажечь LD3" << '\n'
            << "* 30 — потушить LD3" << '\n';
}

int
main(int nargs, char* args[]) {
  using namespace boost;
  namespace prop = program_options;

  prop::options_description desc{};
  desc.add_options()
    ("help,h", "Как пользоваться программой")
    ("command,c", prop::value<std::string>(), "Команда серверу")
    ("addr,a", prop::value<std::string>(), "IP-адрес сервера")
    ("port,p", prop::value<std::uint16_t>(), "Порт")
  ;

  prop::variables_map varmap;

  try {
    const auto parsed{prop::parse_command_line(nargs, args, desc)};
    prop::store(parsed, varmap);

  } catch (std::exception& exception) {
    std::cout << exception.what() << '\n';
    return 1;
  }

  prop::notify(varmap);

  if (varmap.count("help")) {
    std::cout << desc << std::endl;
    show_available_commands();
    return 0;
  }

  if (!varmap.count("command") || !varmap.count("addr") ||
      !varmap.count("port")) {
    std::cout << desc << std::endl;
    show_available_commands();
    return 1;
  }

  const std::string_view command{varmap["command"].as<std::string>()};
  asio::io_context context{};

  const auto server_addr{
      asio::ip::make_address(varmap["addr"].as<std::string>())};
  asio::ip::udp::endpoint endpoint{server_addr,
                                   varmap["port"].as<std::uint16_t>()};
  asio::ip::udp::socket socket{context};

  socket.connect(endpoint);

  auto send_buf{asio::buffer(command)};
  socket.send(send_buf);
}

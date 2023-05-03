#include <boost/asio.hpp>
#include <string_view>

int
main(int nargs, char* args[]) {
  using namespace boost;
  asio::io_context context{};

  const auto server_addr{asio::ip::make_address("192.168.0.10")};
  asio::ip::udp::endpoint endpoint{server_addr, 3333u};
  asio::ip::udp::socket socket{context};

  socket.connect(endpoint);

  const std::string_view to_send{args[1]};
  auto send_buf{asio::buffer(to_send)};
  socket.send(send_buf);
}

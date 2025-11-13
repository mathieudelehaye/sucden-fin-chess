#pragma once

#include <atomic>
#include <thread>

#include "ITransport.hpp"

/**
 * @class PosixTcpTransport
 * @brief Concrete transport implementation using POSIX TCP/IP sockets.
 *
 * This class implements the ITransport interface using a standard
 * POSIX TCP/IP socket. It provides a bidirectional text-based
 * communication channel used by the server to exchange messages with
 * remote clients (console frontend, GUI, etc.).
 *
 * ## Responsibilities
 * - Maintain a connected TCP socket descriptor.
 * - Start a background reading loop.
 * - Notify upper layers when raw text payloads are received.
 * - Deliver outgoing text messages to the connected client.
 * - Handle connection shutdown and cleanup.
 *
 * ## Design
 * The class is part of the transport layer and corresponds to a
 * concrete Strategy in the Strategy pattern. Higher-level components
 * depend only on the abstract ITransport interface. This allows the server
 * to switch transport implementations without modifying the application logic.
 *
 * ## Threading Model
 * A background reader thread is created and cooperatively stopped when the application
 * is exited.
 *
 * The thread:
 * - Continuously read on the socket descriptor.
 * - Converts incoming bytes into messages.
 * - Calls the user-provided ReceiveCallback when data is received.
 * - The write operations are synchronous.
 *
 * ## Lifetime
 * The destructor calls close(), which:
 * - Requests the reading thread to stop.
 * - Shuts down and closes the underlying socket descriptor.
 * This ensures RAII-compliant cleanup.
 */
class PosixTcpTransport : public ITransport {
   public:
    /**
     * @brief Construct a transport from an already-accepted socket descriptor.
     *
     * @param socket_fd File descriptor representing an open TCP connection.
     */
    PosixTcpTransport(int socket_fd);

    /**
     * @brief Destructor closes the socket and stops background threads.
     */
    ~PosixTcpTransport();

    /**
     * @brief Starts the asynchronous reading loop.
     *
     * This method spawns a thread that monitors the socket for incoming data.
     * When data is available, the thread invokes the provided callback.
     *
     * @param onReceive Functor invoked when a full text payload is received.
     */
    void start(ReceiveCallback onReceive) override;

    /**
     * @brief Sends raw text data to the connected client.
     *
     * Internally this writes on the stored socket descriptor.
     *
     * @param data The raw string to send.
     */
    void send(const std::string& data) override;

    /**
     * @brief Closes the transport connection and terminates the reading loop.
     *
     * This method:
     * - Stops the background reading thread.
     * - Shuts down the TCP connection.
     * - Closes the underlying socket file descriptor.
     *
     * After this call, the session and upper layers must treat the connection as
     * closed.
     */
    void close() override;

   private:
    int fd;                            ///< Underlying POSIX socket descriptor.
    std::jthread readerThread;         ///< Background thread reading the socket.
    std::atomic<bool> running{false};  ///< Indicates whether the read loop is active.
};

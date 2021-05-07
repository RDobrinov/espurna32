#ifndef _WSINTERNALFN_H_
#define _WSINTERNALFN_H_
    #include <IPAddress.h>

    #include <cstdint>
    #include <memory>
    #include <vector>

    #include "websockets.hpp"

    // -----------------------------------------------------------------------------
    // WS authentication
    // -----------------------------------------------------------------------------

    struct ws_ticket_t {
        IPAddress ip;
        unsigned long timestamp = 0;
    };

    struct ws_data_t 
    {

        enum mode_t {
            SEQUENCE,
            ALL
        };

        ws_data_t(const ws_on_send_callback_f& cb) :
            storage(new ws_on_send_callback_list_t {cb}),
            client_id(0),
            mode(ALL),
            callbacks(*storage.get()),
            current(callbacks.begin())
        {}

        ws_data_t(uint32_t client_id, const ws_on_send_callback_f& cb) :
            storage(new ws_on_send_callback_list_t {cb}),
            client_id(client_id),
            mode(ALL),
            callbacks(*storage.get()),
            current(callbacks.begin())
        {}

        ws_data_t(const uint32_t client_id, ws_on_send_callback_list_t&& callbacks, mode_t mode = SEQUENCE) :
            storage(new ws_on_send_callback_list_t(std::move(callbacks))),
            client_id(client_id),
            mode(mode),
            callbacks(*storage.get()),
            current(callbacks.begin())
        {}

        ws_data_t(const uint32_t client_id, const ws_on_send_callback_list_t& callbacks, mode_t mode = SEQUENCE) :
            client_id(client_id),
            mode(mode),
            callbacks(callbacks),
            current(callbacks.begin())
        {}

        bool done() {
            return current == callbacks.end();
        }

        void sendAll(JsonObject& root) {
            current = callbacks.end();
            for (auto& callback : callbacks) {
                callback(root);
            }
        }

        void sendCurrent(JsonObject& root) {
            if (current == callbacks.end()) return;
            (*current)(root);
            ++current;
        }

        void send(JsonObject& root) {
            switch (mode) {
                case SEQUENCE: sendCurrent(root); break;
                case ALL: sendAll(root); break;
            }
        }

        std::unique_ptr<ws_on_send_callback_list_t> storage;

        const uint32_t client_id;
        const mode_t mode;
        const ws_on_send_callback_list_t& callbacks;
        ws_on_send_callback_list_t::const_iterator current;
    };

    // -----------------------------------------------------------------------------
    // Debug
    // -----------------------------------------------------------------------------

    using ws_debug_msg_t = std::pair<String, String>;

    struct ws_debug_t 
    {

        ws_debug_t(size_t capacity) :
            flush(false),
            current(0),
            capacity(capacity)
        {
            messages.reserve(capacity);
        }

        void clear() {
            messages.clear();
            current = 0;
            flush = false;
        }

        void add(const char* prefix, const char* message) {
            if (current >= capacity) {
                flush = true;
                send(wsConnected());
            }

            messages.emplace(messages.begin() + current, prefix, message);
            flush = true;
            ++current;
        }

        void send(const bool connected);

        bool flush;
        size_t current;
        const size_t capacity;
        std::vector<ws_debug_msg_t> messages;

    };
#endif /* _WSINTERNALFN_H_ */
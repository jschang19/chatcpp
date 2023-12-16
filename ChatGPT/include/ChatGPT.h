#include <string>
#include "Types.h"
#include <vector>
namespace OpenAI {
    struct Message{
        std :: string role;
        std :: string content;
    };

    class ChatGPT {
        public:
            explicit ChatGPT(const std::string& token);
            OpenAI::ChatCompletion askChatGPT(const std::string& role);
            std::string askWhisper(const std::string& audio_path);
            std::vector< Message > prompts;
        private:
            std::string m_token;
            std::string m_link;

            std::string PromptsToStringContent();
    };
}

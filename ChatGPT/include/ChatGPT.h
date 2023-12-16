#include <string>
#include "Types.h"
#include <vector>
namespace OpenAI {
    class ChatGPT {
    public:
        explicit ChatGPT(const std::string& token);
        OpenAI::ChatCompletion askChatGPT(const std::string& role);
        std::string askWhisper(const std::string& audio_path);
        std::vector< std :: string > prompts;
    private:
        std::string m_token;
        std::string m_link;
    };
}

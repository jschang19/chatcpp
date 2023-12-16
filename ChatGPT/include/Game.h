#include <vector>
#include <string>

namespace System {
    struct UserChoice {
        int story_id;
        std::string content;
    };
    
    class Story {
    public:
        int id;
        std::string place;
        std::string content;
        std::vector< UserChoice > choices;
        Story(int id, std::string place, std::string content)
        : id(id), place(place), content(content) {}
        ~Story(); // destructor
        void addChoice(int story_id, std::string content);
    private:
    };

    class Game {
    public:
        std::vector<int> picked_story_ids;
        Game(); // constructor
        ~Game(); // destructor
        std::vector< Story > stories;
        std::vector< int > getRandStoryIds(int num);
        Story* getStoryPtrById(int id);
        std::vector< Story > readTextFile(const std::string& filename);
    private:
    };
}
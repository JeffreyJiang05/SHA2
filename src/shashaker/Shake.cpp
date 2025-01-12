#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <thread>

#include "Hash.hpp"
#include "hash/SHA.hpp"
#include "shashaker/ArgumentParser.hpp"
#include "shashaker/Process.hpp"

using namespace std;

constexpr int DEFAULT_THREAD_COUNT = 1;

// ./Shaker --format sha512 --wordlist rockyou.txt --hash hash.txt
// ./Shaker -f sha256 -w rockyou.txt -h hash.txt
int main(int argc, const char** argv)
{
    ArgumentParser option_parser{ argc, argv };

    std::regex filename_regex{ "^[A-Za-z0-9_\\. ]+$" };
    std::regex sha_regex{ "^sha(256|224|512|384)$" };
    std::regex numeric_regex{ "^([0-9]+|max)$" };

    const char* wordlist = nullptr;
    const char* hashlist = nullptr;
    const char* sha_type = nullptr;
    // const char* thread_count_str = nullptr;
    int sha_num = 256;
    // int thread_count = DEFAULT_THREAD_COUNT;

    if ((wordlist = option_parser.get_option_argument("-w")))
    {
        if (!std::regex_match(wordlist, filename_regex))
        {
            std::cout << "Incorrectly formatted file name for -w option.\n";
            return 1;
        }
    }
    else if ((wordlist = option_parser.get_option_argument("--wordlist")))
    {
        if (!std::regex_match(wordlist, filename_regex))
        {
            std::cout << "Incorrectly formatted file name for --wordlist option.\n";
            return 1;
        }
    }

    if ((hashlist = option_parser.get_option_argument("-h")))
    {
        if (!std::regex_match(hashlist, filename_regex))
        {
            std::cout << "Incorrectly formatted file name for -h option.\n";
            return 1;
        }
    }
    else if ((hashlist = option_parser.get_option_argument("--hash")))
    {
        if (!std::regex_match(hashlist, filename_regex))
        {
            std::cout << "Incorrectly formatted file name for --hash option.\n";
            return 1;
        }
    }

    if ((sha_type = option_parser.get_option_argument("-f")))
    {
        if (!std::regex_match(sha_type, sha_regex))
        {
            std::cout << "Invalid input for hash type. Only accepts [sha224|sha256|sha384|sha512]\n";
            return 1;
        }

        sha_num = std::stoi(std::string{ sha_type + 3 });
    }
    else if ((sha_type = option_parser.get_option_argument("--format")))
    {
        if (!std::regex_match(sha_type, sha_regex))
        {
            std::cout << "Invalid input for hash type. Only accepts [sha224|sha256|sha384|sha512]\n";
            return 1;
        }

        sha_num = std::stoi(std::string{ sha_type + 3 });
    }

/*     if ((thread_count_str = option_parser.get_option_argument("-t")))
    {
        if (!std::regex_match(thread_count_str, numeric_regex))
        {
            std::cout << "Invalid input for -t option. Requires integral value.\n";
            return 1;
        }

        if (std::strcmp(thread_count_str, "max") == 0)
        {
            thread_count = std::thread::hardware_concurrency();
        }
        else
        {
            thread_count = std::stoi(std::string{ thread_count_str });
        }
    }
    else if ((thread_count_str = option_parser.get_option_argument("--thread")))
    {
        if (!std::regex_match(thread_count_str, numeric_regex))
        {
            std::cout << "Invalid input for --thread option. Requires integral value.\n";
            return 1;
        }

        thread_count = std::stoi(std::string{ thread_count_str });
    } */

    if (!wordlist)
    {
        std::cout << "No word list is provided. Please provide one with the --wordlist or -w options.\n";
        return 1;
    }

    if (!hashlist)
    {
        std::cout << "No hash is provided. Please provide a file containing the hashes to the --hash or -h options.\n";
        return 1;
    }

    std::cout << "Launching Shaker...\n";
    std::cout << "Attempting to crack hashes in " << hashlist << "\n";
    std::cout << "With word list in " << wordlist << "\n";
    std::cout << "Using SHA" << sha_num << "\n\n";
    // std::cout << "Using " << thread_count << " threads\n\n";

    std::ifstream wordlist_file{ wordlist };
    std::ifstream hashlist_file{ hashlist };

    if (wordlist_file.bad())
    {
        std::cout << "Word list file does not exist.\n";
        return 1;
    }

    if (hashlist_file.bad())
    {
        std::cout << "Hash file does not exist.\n";
        return 1;
    }

    switch (sha_num)
    {
        case 224: 
        {
            /* if (thread_count > 1) multithread_sha224_cracker(wordlist_file, hashlist_file, thread_count);
            else  */
            hash_crack_sha224(hashlist_file, wordlist_file);
            break;
        }
        case 256:
        {
            /* if (thread_count > 1) multithread_sha256_cracker(wordlist_file, hashlist_file, thread_count);
            else */ 
            hash_crack_sha256(hashlist_file, wordlist_file);
            break;
        }
        case 384:
        {
            /* if (thread_count > 1) multithread_sha384_cracker(wordlist_file, hashlist_file, thread_count);
            else  */
            hash_crack_sha512(hashlist_file, wordlist_file);
            break;
        }
        case 512:
        {
            /* if (thread_count > 1) multithread_sha512_cracker(wordlist_file, hashlist_file, thread_count);
            else  */
            hash_crack_sha384(hashlist_file, wordlist_file);
            break;
        }
        default:
        {
            std::cout << "Unexpected. This branch should never be taken. Odd.\n";
        }
    }

}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:11:31 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/14 13:03:36 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Servers.hpp"

// Getting all the blocks !!!
int Servers::ConfigFileParse(std::string file)
{
    std::ifstream File(file.c_str());
    if (!File.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return 1;
    }

    std::string line;
    bool insideServerBlock = false;
    std::vector<std::string> block;
    std::stack<char> blockStack;// Stack to keep track of nested blocks
    while (std::getline(File, line))
    { // ! ILA KAN STR KHAWI MATPUSHIHCH L VECTOR
        if (line == "server")
        {
            insideServerBlock = true;
            continue;  // Skip the "server" line
        }
        if (insideServerBlock)
        {
            if (line == "{")
            {
                blockStack.push('{'); // Push a '{' for nested blocks
                continue; // Skip the opening curly brace
            }
            else if (line == "}")
            {
                blockStack.pop(); // Pop a '{' to close the current block
                if (blockStack.empty())
                {
                    insideServerBlock = false;  // Stop when we encounter the closing curly brace
                    // hna ndwz string kamlo (block) bach itparsa f configuration onmchi n9lb 3la next server f configfile
                    Configuration config(block);
                    _servers.push_back(config);
                    for (size_t i = 0; i < block.size(); ++i) {
                        std::cout << block[i] << std::endl;
                    } //ghir bach nchofo
                    std::cout << "--------------------" << std::endl;
                    block.clear(); // Clear the block for the next server
                }
            }
            else
                block.push_back(line);  // Append the line to the block
        }
    }
    File.close();
    return 0;
}

void Servers::printServerData() const {
    for (std::vector<Configuration>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
        std::cout << *it << std::endl;
    }
}
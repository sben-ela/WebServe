/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:11:31 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/08 10:02:44 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/ConfigFile.hpp"

//testing !!

int    ConfigFile::opening(const char *file)
{
    std::ifstream File(file);
    if (!File.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(File, line))
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(line);
        while (std::getline(tokenStream, token))
        {
            if (!token.empty())
                tokens.push_back(token);
        }
        if (!tokens.empty())
        {
            std::string directive = tokens[0];
            std::vector<std::string> values;
            for (size_t i = 1; i < tokens.size(); i++) {
                values.push_back(tokens[i]);
            }
            config[directive] = values;
        }
    }
    File.close();
    return 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/21 19:36:12 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Servers.hpp"
#include "./Includes/Socket.hpp"
//testing !!

int main(int ac, char *av[])
{
    if (ac > 2) {
        std::cerr << "Usage: " << av[0] << " [configuration_file]" << std::endl;
        return 1; // Exit with an error code
    }
    try {
        Servers configFile;
        if (ac == 1)
        {
            std::string file = "defaultconf_file";
            configFile.ConfigFileParse(file);
        }
        else
            configFile.ConfigFileParse(av[1]);
        // ft_Response(Client("GET" , "gif.gif", ".gif",  "image/gif", "HTTP/1.1 ", 1), configFile);
        // Socket s;
        // s.function();
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
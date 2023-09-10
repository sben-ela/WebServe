/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sben-ela <sben-ela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/09 13:17:45 by sben-ela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/ConfigFile.hpp"
#include "./Includes/Socket.hpp"

//testing !!

int main(int ac, char *av[])
{
    if (ac > 2) {
        std::cerr << "Usage: " << av[0] << " [configuration_file]" << std::endl;
        return 1; // Exit with an error code
    }
    ConfigFile configFile;
    if (ac == 1)
    {
        const char *file = "defaultconf_file";
        configFile.opening(file);
    }
    else
        configFile.opening(av[1]);
    Socket s;
    s.function();
    return 0;
}

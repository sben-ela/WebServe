/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:22:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/05 17:18:27 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
// #include "../Webserv.hpp"

// class Location : public CommonEntity
// {
//     public:
//         typedef void (Location::*methods)(std::string);
//     private:
//         std::string                        _pattern;
//         std::vector<std::string>           _limit_except;
//         std::map<std::string, std::string> _cgi;
//         std::string                        _upload;
//         std::string                        _redirect;
//         bool                               _pattren_exists;
//     public:
//         Location();
//         Location(const CommonEntity& base, TokenVectsIter& begin, TokenVectsIter& end);
//         Location(const Location& other);
//         Location&                    operator=(const Location& other);
//         void                         InitPattern(std::string value);
//         void                         InitLimitExcept(std::string value);
//         void                         InitCgi(std::string value);
//         void                         InitUpload(std::string value);
//         void                         InitRedirect(std::string value);
//         std::string                  getPattren() const;
//         std::vector<std::string>     getLimit_except() const;
//         std::map<std::string, std::string>             getCgi() const;
//         std::string                  getUpload() const;
//         std::string                  getRedirect() const;
//         ~Location();
//         friend std::ostream& operator<<(std::ostream& o, Location obj);
// };
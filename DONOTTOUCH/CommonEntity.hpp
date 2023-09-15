/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonEntity.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:20:29 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 13:34:28 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"

class CommonEntity
{
    public:
        typedef void (CommonEntity::*methods)(std::string);
    protected:
        std::string                 _root;
        std::vector<std::string>    _index;
        std::map<int, std::string>  _error_pages;
        size_t                      _client_max_body_size;
        bool                        _AutoIndex;
        bool                        _root_exits;
    public:
        CommonEntity();
        CommonEntity(TokenVectsIter begin, TokenVectsIter end);
        CommonEntity(const CommonEntity& other);
        CommonEntity& operator=(const CommonEntity& other);
        void                         InitRoot(std::string value);
        void                         InitIndex(std::string value);
        void                         InitErrorPage(std::string value);
        void                         InitClienBodySize(std::string value);
        void                         InitAutoIndex(std::string value);
        std::string                  getRoot() const;
        std::vector<std::string>     getIndex() const;
        std::map<int, std::string>   getErrorPages() const;
        size_t                       getClientMaxBodySize() const;
        bool                         getAutoIndex() const;
};
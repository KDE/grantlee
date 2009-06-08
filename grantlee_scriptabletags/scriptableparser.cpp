/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 3 only, as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License version 3 for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "scriptableparser.h"
#include "scriptablenode.h"

#include "parser.h"


ScriptableParser::ScriptableParser( Grantlee::Parser* p, QObject* parent )
    : QObject( parent ), m_p( p )
{
}

void ScriptableParser::deleteNextToken()
{
  m_p->deleteNextToken();
}

bool ScriptableParser::hasNextToken() const
{
  return m_p->hasNextToken();
}

void ScriptableParser::loadLib( const QString& name )
{
  m_p->loadLib( name );
}

Token ScriptableParser::nextToken()
{
  return m_p->nextToken();
}

void ScriptableParser::skipPast( const QString& tag )
{
  m_p->skipPast( tag );
}

QObjectList ScriptableParser::parse( const QString& stopAt, QObject* parent )
{
  return parse( QStringList() << stopAt, parent );
}

QObjectList ScriptableParser::parse( const QStringList& stopAt, QObject* parent )
{
  NodeList nodeList = m_p->parse( stopAt, parent );
  QObjectList objList;
  QListIterator<Node*> it( nodeList );
  while ( it.hasNext() ) {
    objList << it.next();
  }
  return objList;
}

QObjectList ScriptableParser::parse( QObject* parent )
{
  return parse( QStringList(), parent );
}

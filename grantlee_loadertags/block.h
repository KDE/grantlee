/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>
*/

#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "node.h"
#include "interfaces/taglibraryinterface.h"


namespace Grantlee
{
class Parser;
}

using namespace Grantlee;

class BlockNodeFactory : public AbstractNodeFactory
{
  Q_OBJECT
public:
  BlockNodeFactory(QObject *parent = 0);

  Node* getNode(const QString &tagContent, Parser *p, QObject *parent) const;

};

class BlockNode : public Node
{
  Q_OBJECT
  Q_PROPERTY(QString super READ getSuper)
public:
  BlockNode(const QString &blockName, const NodeList &list, QObject *parent);

  QString blockName();

  QString render(Context *c);

  NodeList getNodesByType(const char * className);

  BlockNode* nodeParent() const;

  QString name();

  void setNodeParent(BlockNode *);

  void addParent(NodeList);

  void setNodeList(NodeList);
  NodeList nodeList();

// slots
  /**
  Returns the block overriden by this one rendered in context.
  */
  QString getSuper() const;

private:
//   FilterExpression m_filterExpression;
  QString m_name;
  NodeList m_list;
  Context *m_context;
  BlockNode *m_parent;

};

#endif

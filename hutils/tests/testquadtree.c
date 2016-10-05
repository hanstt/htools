/*
 * Copyright (c) 2016 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <hutils/quadtree.h>
#include <htest/htest.h>
#include <hutils/memory.h>

struct TestNode {
	QUADTREE_ENTRY(TestNode)	node;
};
struct TestData {
	int	creation_level;
	int	node_num;
};
QUADTREE_HEAD(TestTree, TestNode, TestData);

static struct TestNode		*node_alloc(struct TestData *) FUNC_RETURNS;
static void			node_free(struct TestData *, struct TestNode
    **);
static enum QuadtreeTraversal	node_visitor(struct TestData *, struct
    TestNode *) FUNC_RETURNS;
QUADTREE_PROTOTYPE(testtree, TestTree, TestNode, TestData);

QUADTREE_IMPLEMENT(testtree, TestTree, TestNode, TestData, node_alloc,
    node_free, node_visitor, node);

struct TestNode *
node_alloc(struct TestData *a_user)
{
	struct TestNode *node;

	CALLOC(node, 1);
	++a_user->node_num;
	return node;
}

void
node_free(struct TestData *a_user, struct TestNode **a_node)
{
	struct TestNode *node;

	node = *a_node;
	ASSERT(void *, "p", NULL, !=, node);
	ASSERT(void *, "p", NULL, ==, node->node.child[0]);
	FREE(*a_node);
	ASSERT(int, "d", 0, <, a_user->node_num);
	--a_user->node_num;
}

enum QuadtreeTraversal
node_visitor(struct TestData *a_user, struct TestNode *a_node)
{
	if (a_user->creation_level > a_node->node.level) {
		return QUADTREE_CONTINUE;
	}
	return QUADTREE_CLEAR;
}

HTEST(Root)
{
	struct TestData data;
	struct TestTree tree;

	HTRY_SIGNAL(testtree_traverse(&tree));

	ZERO(data);
	testtree_init(&tree, &data, 8.0f);
	HTRY_I(1, ==, data.node_num);
	HTRY_PTR(NULL, !=, tree.root);
	HTRY_FLT(0, ==, tree.root->node.x);
	HTRY_FLT(0, ==, tree.root->node.y);

	HTRY_VOID(testtree_traverse(&tree));
	HTRY_I(1, ==, data.node_num);
	HTRY_PTR(NULL, !=, tree.root);

	testtree_shutdown(&tree);
	HTRY_I(0, ==, data.node_num);
	HTRY_PTR(NULL, ==, tree.root);

	HTRY_SIGNAL(testtree_traverse(&tree));
}

HTEST(Creating)
{
	struct TestData data;
	struct TestTree tree;

	ZERO(data);
	testtree_init(&tree, &data, 8.0f);

	data.creation_level = 1;
	testtree_traverse(&tree);
	HTRY_I(1 + 4, ==, data.node_num);
	HTRY_FLT(0, ==, tree.root->node.child[0]->node.x);
	HTRY_FLT(0, ==, tree.root->node.child[0]->node.y);
	HTRY_FLT(1, ==, tree.root->node.child[1]->node.x);
	HTRY_FLT(0, ==, tree.root->node.child[1]->node.y);
	HTRY_FLT(0, ==, tree.root->node.child[2]->node.x);
	HTRY_FLT(1, ==, tree.root->node.child[2]->node.y);
	HTRY_FLT(1, ==, tree.root->node.child[3]->node.x);
	HTRY_FLT(1, ==, tree.root->node.child[3]->node.y);

	data.creation_level = 2;
	testtree_traverse(&tree);
	HTRY_I(1 + 4 + 16, ==, data.node_num);

	data.creation_level = 3;
	testtree_traverse(&tree);
	HTRY_I(1 + 4 + 16 + 64, ==, data.node_num);

	data.creation_level = 1;
	testtree_traverse(&tree);
	HTRY_I(1 + 4, ==, data.node_num);

	testtree_shutdown(&tree);
	HTRY_I(0, ==, data.node_num);
}

HTEST_SUITE(Quadtree)
{
	HTEST_ADD(Root);
	HTEST_ADD(Creating);
}
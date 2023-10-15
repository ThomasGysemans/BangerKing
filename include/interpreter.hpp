#pragma once

#include "runtime.hpp"
#include "context.hpp"
#include "nodes/compositer.hpp"
#include "values/compositer.hpp"
using namespace std;

class Interpreter {
  public:
    const RuntimeResult* visit(const CustomNode* node, const Context& ctx) const;
    
  private:
    const RuntimeResult* visit_NumberNode(const NumberNode* node, const Context& ctx) const;

    // RuntimeResult* visit_AddNode(const AddNode* node, const Context& ctx) {
    //   RuntimeResult* res = new RuntimeResult();
    //   Value<>* left = res->read(visit(node->get_a(), ctx));
    //   if (res->should_return()) return res; // if there was an error for example, we should quit now
    //   Value<>* right = res->read(visit(node->get_b(), ctx));
    //   if (res->should_return()) return res;

    //   if (instanceof<IntegerValue>(left) && instanceof<IntegerValue>(right)) {
    //     IntegerValue* a = dynamic_cast<IntegerValue*>(left);
    //     IntegerValue* b = dynamic_cast<IntegerValue*>(right);
    //     IntegerValue r = (*a) + (*b);
    //     r.set_pos(node->getStartingPosition(), node->getEndingPosition());
    //     r.set_ctx(&ctx);
    //     Value<>* v = dynamic_cast<Value<>*>(&r);
    //     return (new RuntimeResult())->success(v);
    //   }

    //   throw RuntimeError(
    //     node->getStartingPosition(), node->getEndingPosition(),
    //     "Illegal operation",
    //     &ctx
    //   );
    // }
};

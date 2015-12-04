      for (int idx = 0; idx < inst_index.size(); ++idx) {
        Instruction *inst = inst_index[idx];
        uint32_t callid;
        CallInst *callinst;

        if (setCalleeID(inst, &callid, &callinst)) {
          if (callid == CAT_get_signed_value_ID) {
            Value *var = callinst->getArgOperand(0);
            Instruction *var_inst = dyn_cast<Instruction>(var);
            std::set<Instruction *> &IN = IN_sets[idx];
            Instruction *reachingDef = NULL;
            //errs() << *inst << '\n';
            //errs() << *var << '\n';
            // check IN set elements
            //errs() << "IN: " << '\n';
            for (std::set<Instruction *>::iterator iter = IN.begin(); iter != IN.end(); ++iter) {
              //errs() << **iter << '\n';
              bool done = false;
              uint32_t id;
              CallInst *cinst;
              if (!setCalleeID(*iter, &id, &cinst)) {
                //errs() << "i am here 1\n";
                if (dyn_cast<PHINode>(*iter) == var) {
                  // phi node can have at least 2 operand
                  // merge multiple reaching definition
                  auto phi_node_ptr = dyn_cast<PHINode>(*iter);

                  unsigned num_in_value = phi_node_ptr->getNumIncomingValues();
                  unsigned count = 0;

                  Value *first_val, *second_val;
                  uint32_t first_id, second_id;
                  CallInst *first_call_inst, *second_call_inst;
                  while (count + 1 < num_in_value) {
                    first_val = phi_node_ptr->getIncomingValue(count);
                    second_val = phi_node_ptr->getIncomingValue(count + 1);
                    if (auto first_inst = dyn_cast<Instruction>(first_val)) {
                      if (auto second_inst = dyn_cast<Instruction>(second_val)) {
                        if (setCalleeID(first_inst, &first_id, &first_call_inst) && setCalleeID(second_inst, &second_id, &second_call_inst)) {
                          if (first_id == CAT_create_signed_value_ID && second_id == CAT_create_signed_value_ID) {
                            if (first_call_inst->getArgOperand(0) == second_call_inst->getArgOperand(0)) {
                             reachingDef = first_inst;
                             ++count;
                            } else {
                              reachingDef = NULL;
                              break;
                            }
                          } else {
                            reachingDef = NULL;
                            break;
                          }
                        } else {
                          reachingDef = NULL;
                          break;
                        }
                      } else {
                        reachingDef = NULL;
                        break;
                      }
                    } else {
                      reachingDef = NULL;
                      break;
                    }
                  }                  
                  continue;
                } else if (fake_insts_to_arg.find(*iter) != fake_insts_to_arg.end()) {
                  // if iter is fake_instruction
                  //errs() << "i am fake inst\n";
                  if (fake_insts_to_arg[*iter] == var) {
                    //errs() << "i am killing var\n";
                    reachingDef = NULL;
                    break;
                  } else {
                    // not match
                    // do nothing
                    continue;
                  }
                } else {
                  // in instruction neither fake_inst nor phi node 
                  //errs() << "neither \n"; 
                  //errs() << *iter << '\n';
                  continue;
                }
                //errs() << "i am here\n";

              } else {
              // if is CAT funcions
                switch (id) {
                  case CAT_create_signed_value_ID: {
                    if (var_inst == *iter) {
                      reachingDef = *iter;
                    }
                  break;
                  }
                  case CAT_binary_add_ID: {
                   // binary_add first arg
                    auto a_def_var = cinst->getArgOperand(0);
                    if (var == a_def_var) {
                      reachingDef = NULL;
                      done = true;
                     } else {
                      if (esc_vars.find(var) != esc_vars.end()) {
                        auto deps_ptr = deps.depends(dyn_cast<Instruction>(a_def_var), var_inst, false);
                        if (deps_ptr != NULL) {
                          //errs() << *deps_ptr->getSrc() << *deps_ptr->getDst() << '\n';
                          if (deps_ptr->isOrdered()) {
                            reachingDef = NULL;
                            done = true;
                          } else {
                            // input deps, do nothing
                          }
                        } else {
                        // no depends, do nothing
                        }
                      } else {
                      // if var not escape memory, never data check deps
                      }
                    }
                    break;
                  }
                  case CAT_get_signed_value_ID: {
                    break;
                  }
                }

                if (done) {
                  break;
                }
            }    
          }
            // constant propagate
            //int64_t c;
            if (reachingDef != NULL) {
              CallInst *v_to_c = dyn_cast<CallInst>(reachingDef);
              Value *v = v_to_c->getArgOperand(0);
              if (isa<ConstantInt>(v)) {
                ConstantInt *cnst = dyn_cast<ConstantInt>(v);
                BasicBlock::iterator ii(inst);
                ReplaceInstWithValue(inst->getParent()->getInstList(), ii, cnst);
              }
            }
          }
        }

      }
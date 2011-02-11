/***************************************************************************\
|* Function Parser for C++ v4.3                                            *|
|*-------------------------------------------------------------------------*|
|* Function optimizer                                                      *|
|*-------------------------------------------------------------------------*|
|* Copyright: Joel Yliluoma                                                *|
|*                                                                         *|
|* This library is distributed under the terms of the                      *|
|* GNU Lesser General Public License version 3.                            *|
|* (See lgpl.txt and gpl.txt for the license text.)                        *|
\***************************************************************************/

/* NOTE:
 This file contains generated code (from the optimizer sources) and is
 not intended to be modified by hand. If you want to modify the optimizer,
 download the development version of the library.
*/

#include "fpconfig.hh"
#ifdef FP_SUPPORT_OPTIMIZER
#include "fparser.hh"
#include "fptypes.hh"
#define iS3 (param
#define iR3 info.SaveMatchedParamIndex(
#define iQ3 xX info
#define iP3 iH l7 a),
#define iO3 if(xO)&&
#define iN3 "PUSH ";
#define iM3 "Found "
#define iL3 stackpos
#define iK3 ),c11 xM
#define iJ3 "dup(%u) "
#define iI3 "%d, cost "
#define iH3 "immed "<<
#define iG3 mFuncParsers
#define iF3 cZ2 assert
#define iE3 stderr
#define iD3 sep2=" "
#define iC3 param.data
#define iB3 FPHASH_CONST
#define iA3 cache_needed[
#define i93 fprintf
#define i83 "Applying "
#define i73 ||tree.GetOpcode
#define i63 HANDLE_UNARY_CONST_FUNC
#define i53 within,
#define i43 );iB i0
#define i33 .iY1 a);
#define i23 .empty()
#define i13 yW(0)&&
#define i03 c_count
#define tZ3 s_count
#define tY3 MaxOp
#define tX3 2)lP 2*
#define tW3 tmp.cQ1
#define tV3 <yW(0)?
#define tU3 x23.eR1
#define tT3 ,eP,cV1;
#define tS3 ,iR1&i3
#define tR3 AddFrom(
#define tQ3 (p1 nJ1)
#define tP3 middle2
#define tO3 b.Value)
#define tN3 b.Opcode
#define tM3 )const
#define tL3 (tM3{
#define tK3 e9 mulgroup
#define tJ3 )e9 iB1)
#define tI3 )min.set(
#define tH3 .n9 synth.
#define tG3 (list.first
#define tF3 .end()
#define tE3 return p
#define tD3 }return
#define tC3 else{if
#define tB3 t63;cE3
#define tA3 nL3.ofs
#define t93 1),yW(1));
#define t83 (IfData&nL3
#define t73 fp_const_pihalf<yW>()
#define t63 <yW>())
#define t53 <yW(0))
#define t43 max.val
#define t33 iL);}if
#define t23 for e21
#define t13 sim.nV1
#define t03 ].swap(
#define eZ3 =synth.
#define eY3 codes[b
#define eX3 Value){
#define eW3 ),Value
#define eV3 eU3"\n";
#define eU3 ,o);o<<
#define eT3 whydump
#define eS3 &*iL1){
#define eR3 *x7 tX1
#define eQ3 for(;a<
#define eP3 nparams
#define eO3 l4 4,1,
#define eN3 491,{2,
#define eM3 426,{2,
#define eL3 cTan,yY
#define eK3 {data->
#define eJ3 l3 0,1,
#define eI3 cLog,yY
#define eH3 2,444848,
#define eG3 l4 0,1,
#define eF3 0x12 y7
#define eE3 cHypot,
#define eD3 xO)){yW tmp=
#define eC3 (xO nJ
#define eB3 eT 1,0,
#define eA3 cAbs,x6
#define e93 0x4 y7
#define e83 0x4},{{1,
#define e73 y3 lT 2,
#define e63 },0,0x4
#define e53 iD2 63)-1;
#define e43 ;if iD2
#define e33 yW>(),xO1
#define e23 yW>(eU1
#define e13 base,yW
#define e03 false;}
#define cZ3 leaf1
#define cY3 cAbsIf)
#define cX3 l01++b)
#define cW3 );lD yQ
#define cV3 yP r);}
#define cU3 Ne_Mask
#define cT3 Gt_Mask
#define cS3 Lt_Mask
#define cR3 iS1 nI1
#define cQ3 FindPos
#define cP3 {yW cA3
#define cO3 };enum
#define cN3 ;}void
#define cM3 )cN3
#define cL3 Immeds
#define cK3 [funcno].
#define cJ3 (eC==tQ2&&
#define cI3 public:
#define cH3 iL1,info
#define cG3 result
#define cF3 {unsigned
#define cE3 cG3 xQ
#define cD3 cG3 yS
#define cC3 )cG3 i61
#define cB3 cG3.min
#define cA3 cG3(
#define c93 }c6 cN2
#define c83 l2 0,2,
#define c73 pclone
#define c63 ,cPow)
#define c53 stack.
#define c43 params
#define c33 cN 1,
#define c23 cOr,l6
#define c13 newpow
#define c03 (op1==
#define yZ3 (cG3
#define yY3 {}range(
#define yX3 (count
#define yW3 133,2,
#define yV3 Params
#define yU3 size_t
#define yT3 Needs
#define yS3 byteCode
#define yR3 lG1 nE==
#define yQ3 cLog2by
#define yP3 factor_t
#define yO3 value1
#define yN3 p2 cJ ifp2
#define yM3 xA p2;p2
#define yL3 cAbsNot
#define yK3 stackptr
#define yJ3 iB[++IP]
#define yI3 yW(0.5))
#define yH3 lT 1,0,
#define yG3 ,lT 2,
#define yF3 stack[c53
#define yE3 stack i0
#define yD3 size()-1
#define yC3 iB[IP]==
#define yB3 opcodes
#define yA3 did_muli
#define y93 break;}}
#define y83 used[b]
#define y73 yU3 n
#define y63 iO.hash1
#define y53 sizeof(
#define y43 cAbsIf,
#define y33 cNotNot,
#define y23 281856,
#define y13 l2 2,1,
#define y03 const xG
#define xZ3 if iS3
#define xY3 xD1;case
#define xX3 Ge0Lt1
#define xW3 Gt0Le1
#define xV3 =fp_pow(
#define xU3 .data.n1
#define xT3 switch(lT3.first iK2
#define xS3 xF1){yW
#define xR3 ;for e51
#define xQ3 .what nB1
#define xP3 ==cOr)l8
#define xO3 cAdd iX1
#define xN3 iterator
#define xM3 begin();
#define xL3 parent
#define xK3 insert(i
#define xJ3 change=
#define xI3 newrel
#define xH3 void set
#define xG3 n92 yW&v
#define xF3 synth xE2
#define xE3 synth.xL
#define xD3 b_needed
#define xC3 n81&cV1{
#define xB3 cachepos
#define xA3 ,bool abs)
#define x93 1,iS+1);
#define x83 half=
#define x73 src_pos
#define x63 reserve(
#define x53 iK1 tree
#define x43 nI1 void
#define x33 treeptr
#define x23 );m.max
#define x13 iU2 size()
#define x03 cC cMul);
#define nZ3 l0 y02
#define nY3 .resize(
#define nX3 xC1 xA>&
#define nW3 eS1 void
#define nV3 ImmedTag
#define nU3 a,const
#define nT3 RefCount
#define nS3 Birth();
#define nR3 exponent
#define nQ3 template
#define nP3 cost_t
#define nO3 iftree
#define nN3 fpdata
#define nM3 middle
#define nL3 ifdata
#define nK3 TreeCountItem
#define nJ3 if(op==
#define nI3 ,yW(1))){
#define nH3 sqrt_cost
#define nG3 const int
#define nF3 =iZ root l7
#define nE3 cG3=
#define nD3 mul_count
#define nC3 maxValue1
#define nB3 minValue1
#define nA3 maxValue0
#define n93 minValue0
#define n83 ValueType
#define n73 cG known
#define n63 cG n4 0),
#define n53 max.known
#define n43 pos_set
#define n33 cC cond nE
#define n23 tree cC
#define n13 cLog);x82
#define n03 yN 2,cAdd
#define lZ3 a;if(&iO1
#define lY3 (cond c4
#define lX3 DumpTree(
#define lW3 subtree
#define lV3 invtree
#define lU3 =(*x7)[a]
#define lT3 parampair
#define lS3 rulenumit
#define lR3 l0 2,
#define lQ3 MakeEqual
#define lP3 ,cAnd,l6
#define lO3 lN3 IsAlways;if(
#define lN3 ))return
#define lM3 ,const e1&
#define lL3 range<yW nT
#define lK3 range<yW>
#define lJ3 newbase
#define lI3 fp_equal(
#define lH3 branch1op
#define lG3 branch2op
#define lF3 overlap
#define lE3 ));i21 1,
#define lD3 truth_b
#define lC3 truth_a
#define lB3 found_dup
#define lA3 lG r;r cC
#define l93 rangeutil
#define l83 Plan_Has(
#define l73 const nT2
#define l63 namespace
#define l53 std::endl;
#define l43 const std::eO
#define l33 const char*
#define l23 ContainsOtherCandidates(
#define l13 std::cout
#define l03 inverted
#define iZ2 IsLogicalValue iH l7
#define iY2 IsNever:
#define iX2 >(yW(1),
#define iW2 },0,0x1},{{1,
#define iV2 ,l1 0x4},{{3,
#define iU2 yV3.
#define iT2 .size();++
#define iS2 yS&&p xN
#define iR2 .known&&
#define iQ2 depcodes
#define iP2 explicit
#define iO2 cCosh,x6
#define iN2 VarBegin
#define iM2 ));TriTruthValue
#define iL2 iB i0 lP2|iK)
#define iK2 ){case
#define iJ2 ].data);
#define iI2 xK)));nW
#define iH2 begin(),
#define iG2 cond_add
#define iF2 cond_mul
#define iE2 cond_and
#define iD2 (half&
#define iC2 bool cS1
#define iB2 start_at
#define iA2 costree
#define i92 sintree
#define i82 leaf_count
#define i72 ),child);
#define i62 ;t1=!t1;}}
#define i52 sub_params
#define i42 tree l7 0)
#define i32 printf(
#define i22 swap(tmp);
#define i12 cbrt_count
#define i02 sqrt_count
#define tZ2 .PullResult(
#define tY2 );sim.Push(
#define tX2 )lP 3*3*
#define tW2 if iH nE==
#define tV2 );tmp.nL 0));
#define tU2 ;DumpTree yR
#define tT2 ,cNot,x6
#define tS2 l7 1)xF1&&
#define tR2 PlusInf
#define tQ2 Finite
#define tP2 ,l13
#define tO2 p1 yP p1);
#define tN2 p1 cJ ifp1
#define tM2 switch(tB
#define tL2 }switch iH
#define tK2 pcall_tree
#define tJ2 after_powi
#define tI2 info.lO[b].
#define tH2 info=info;
#define tG2 ;synth.c2
#define tF2 yZ e03
#define tE2 ):iB2()
#define tD2 c43)
#define tC2 grammar
#define tB2 cEqual,
#define tA2 cLog,x6
#define t92 cNeg,lT 1,
#define t82 data;data.
#define t72 MakeNEqual
#define t62 Dump(std::
#define t52 isInteger(
#define t42 Comparison
#define t32 needs_flip
#define t22 .Remember(
#define t12 value]
#define t02 );goto redo;
#define eZ2 >StackMax)
#define eY2 ~yU3(0)
#define eX2 x21 xD+1);
#define eW2 >::res,b8<
#define eV2 cJ tree);
#define eU2 mul_item
#define eT2 innersub
#define eS2 cbrt_cost
#define eR2 best_cost
#define eQ2 fp_max(cE
#define eP2 tree nE==
#define eO2 ;sim.x0 2
#define eN2 condition
#define eM2 per_item
#define eL2 item_type
#define eK2 first2
#define eJ2 ,cCos,yY
#define eI2 cGreater,
#define eH2 (cZ3 l7
#define eG2 cIf,eT 3,
#define eF2 i1 1},0,
#define eE2 Decision
#define eD2 not_tree
#define eC2 group_by
#define eB2 nR3=
#define eA2 ->second.
#define e92 ;}static yD1
#define e82 ,cPow,yY
#define e72 for(typename
#define e62 ,nY1);lD
#define e52 ;std::vector<
#define e42 xM1.SubTrees
#define e32 xM1.Others
#define e22 for e51 0;a<
#define e12 :sim cV2
#define e02 tree l7 1)
#define cZ2 xA&tree){
#define cY2 targetpos
#define cX2 eat_count
#define cW2 ParamSpec
#define cV2 .Eat(1,
#define cU2 ;iO.hash2+=
#define cT2 .GetHash().
#define cS2 ;}case
#define cR2 nD OPCODE
#define cQ2 ,unsigned
#define cP2 rhs.hash2;}
#define cO2 rhs.hash1
#define cN2 struct
#define cM2 Forget()
#define cL2 &&cond eE))
#define cK2 source_tree
#define cJ2 .n_int_sqrt
#define cI2 nE==cLog2&&
#define cH2 <iF,nP3>
#define cG2 CodeTree lW
#define cF2 p1_evenness
#define cE2 isNegative(
#define cD2 .max.n4 0),
#define cC2 .constvalue
#define cB2 .min.known=false;if(
#define cA2 (cG val
#define c92 if iH l7 a)
#define c82 cNop,cNop}}
#define c72 cTanh,cNop,
#define c62 matches
#define c52 ;}else{x7=new
#define c42 iK index)yZ
#define c32 tree xF1 y5
#define c22 goto fail;}
#define c12 (std::move(
#define c02 ;n6 lD
#define yZ2 l7 0));tree
#define yY2 n92 void*)&
#define yX2 cSin,yY
#define yW2 cTan,x6
#define yV2 cCos,x6
#define yU2 if iH l7 0)
#define yT2 negated
#define yS2 factor
#define yR2 coshtree
#define yQ2 sinhtree
#define yP2 best_score
#define yO2 mulvalue
#define yN2 pow_item
#define yM2 subgroup
#define yL2 nE==cPow&&tC
#define yK2 PowiResult
#define yJ2 >=yW(0)
#define yI2 maxValue
#define yH2 minValue
#define yG2 set(fp_floor
#define yF2 fp_min(cE,
#define yE2 div_tree
#define yD2 pow_tree
#define yC2 preserve
#define yB2 dup_or_fetch
#define yA2 nominator]
#define y92 .SetParamsMove(
#define y82 Rehash(false
#define y72 test_order
#define y62 lT3,
#define y52 .param_count
#define y42 shift(index)
#define y32 {l13<<
#define y22 rulenumber
#define y12 cSinh,x6
#define y02 2,7168,
#define xZ2 TopLevel)
#define xY2 cInv,lT 1,
#define xX2 break;}switch(
#define xW2 constraints&
#define xV2 constraints=
#define xU2 ;nW l5::
#define xT2 }cP xR2 t9 cP
#define xS2 .GetDepth()
#define xR2 xE,l5::
#define xQ2 factor_immed
#define xP2 changes
#define xO2 cJ leaf2 l7
#define xN2 cJ cZ3 l7
#define xM2 cJ cond l7
#define xL2 tree l7 a)
#define xK2 exp_diff
#define xJ2 ExponentInfo
#define xI2 lower_bound(
#define xH2 is_logical
#define xG2 newrel_and
#define xF2 );p2 yP p2)cU
#define xE2 .AddOperation(
#define xD2 .UseGetNeeded(
#define xC2 );break;}
#define xB2 ,cTanh,x6
#define xA2 ,eJ1 0x5 y7
#define x92 yW>(),0},{
#define x82 sim.xT
#define x72 eL[c tO
#define x62 +=1 xD1 lN1;
#define x52 res_stackpos
#define x42 half_pos
#define x32 ,1,2,1,
#define x22 >>1)):(
#define x12 CodeTreeData
#define x02 0x0},{
#define nZ2 Constness_Const
#define nY2 },0,x02{
#define nX2 l13<<"POP "
#define nW2 nR3)
#define nV2 var_trees
#define nU2 MakeFalse
#define nT2 CodeTree&
#define nS2 parent_opcode
#define nR2 =GetParam
#define nQ2 .match_tree
#define nP2 changed=true;
#define nO2 log2_exponent
#define nN2 lG tmp;tmp cC
#define nM2 .back().thenbranch
#define nL2 (yS2
#define nK2 0));lK3
#define nJ2 dup_fetch_pos
#define nI2 IsNever y5 lD
#define nH2 cSin,x6
#define nG2 Value_EvenInt
#define nF2 (rule,tree,info
#define nE2 2*2*2)lP 3
#define nD2 n92 Rule&rule,
#define nC2 else if(
#define nB2 ));lM1 cJ y1 l7
#define nA2 Value_Logical
#define n92 (const
#define n82 param=*n92
#define n72 ParamHolder:{
#define n62 y8 DelParams(
#define n52 ConditionType
#define n42 DUP_ONE(apos)
#define n32 yS=false
#define n22 SpecialOpcode
#define n12 AddParamMove(
#define n02 =i eA2
#define lZ2 IsDefined()
#define lY2 xG1 xW++a){
#define lX2 assimilated
#define lW2 denominator
#define lV2 fraction
#define lU2 DUP_BOTH();
#define lT2 lN3 IsNever xD1 Unknown;}
#define lS2 ,i3 tT3
#define lR2 [tA3
#define lQ2 -1-offset].
#define lP2 0x80000000u
#define lO2 >::Optimize(){
#define lN2 .erase(cs_it);
#define lM2 synth.Find
#define lL2 TreeCounts
#define lK2 bool t1=false;
#define lJ2 SetOpcode(
#define lI2 found_log2
#define lH2 div_params
#define lG2 );tmp2.nL
#define lF2 minimum_need
#define lE2 const yW&value
#define lD2 case IsAlways:
#define lC2 p0.min iR2
#define lB2 immed_sum
#define lA2 OPCODE(opcode)
#define l92 break;cG3*=
#define l82 iB2;
#define l72 IsAlways y5 lD
#define l62 y33 x6
#define l52 DumpHashesFrom
#define l42 replacing_slot
#define l32 RefParams
#define l22 WhatDoWhenCase
#define l12 exponent_immed
#define l02 new_base_immed
#define iZ1 base_immed
#define iY1 DelParam(
#define iX1 ||op1==
#define iW1 data[a].second
#define iV1 AddCollection(
#define iU1 .AddItem(atree
#define iT1 if(newrel_or==
#define iS1 (x53)
#define iR1 PowiCache
#define iQ1 e6 2,131,
#define iP1 cC tree nE);
#define iO1 *iB2){x7=(
#define iN1 ++a)if(remaining[a])
#define iM1 for(yU3 b=0;b<
#define iL1 (*x7)[a].iB2
#define iK1 const xA&
#define iJ1 OptimizedUsing
#define iI1 Var_or_Funcno
#define iH1 iI1;
#define iG1 GetParams(
#define iF1 crc32_t
#define iE1 signed_chain
#define iD1 if(lI3
#define iC1 MinusInf
#define iB1 divgroup
#define iA1 GetOpcode())
#define i91 FactorStack<yW>
#define i81 {if(GetOpcode()
#define i71 ,eO3 556,{2,
#define i61 .min.val
#define i51 Value(Value::
#define i41 )val=func l91
#define i31 synth.PushImmed(
#define i21 tree.SetParam(
#define i11 );synth.StackTopIs(
#define i01 xC1 unsigned>&iB,yU3&IP,yU3 t91
#define tZ1 .Rehash();
#define tY1 public e5,public std::vector<
#define tX1 )[a].iB2=r.specs;if(r.found){
#define tW1 return true;
#define tV1 n_immeds
#define tU1 FindClone(xI
#define tT1 needs_rehash
#define tS1 AnyWhere_Rec
#define tR1 ~unsigned(0)
#define tQ1 41,42,43,44,
#define tP1 .constraints
#define tO1 &&if_always[
#define tN1 p1_logical_b
#define tM1 p0_logical_b
#define tL1 p1_logical_a
#define tK1 p0_logical_a
#define tJ1 synth.DoDup(
#define tI1 cache_needed
#define tH1 e6 2,1,e6 2,
#define tG1 treelist
#define tF1 FindAndDup iH);
#define tE1 IsDescendantOf(
#define tD1 has_bad_balance
#define tC1 yP3 yS2
#define tB1 fp_abs(t43))
#define tA1 fp_abs(min.val)
#define t91 limit,yU3 y0
#define t81 cNEqual
#define t71 i1 2 nY2
#define t61 Oneness_NotOne|
#define t51 Value_IsInteger
#define t41 iJ1(
#define t31 .Become iH l7
#define t21 mulgroup.
#define t11 .n12
#define t01 std::vector<xA>
#define eZ1 ;tree.iY1
#define eY1 reltype
#define eX1 SequenceOpcodes
#define eW1 y9 Immed.size());
#define eV1 +1 y9 iB.size()
#define eU1 ),0},{yW(
#define eT1 nQ3<
#define eS1 l32);
#define eR1 nQ3 set_if<
#define eQ1 nQ3 lY
#define eP1 xA tmp;tmp cC
#define eO1 MaxChildDepth
#define eN1 repl_param_list,
#define eM1 situation_flags&
#define eL1 std::pair<It,It>
#define eK1 282870,{2,
#define eJ1 cPow,lA
#define eI1 Sign_Negative
#define eH1 new_factor_immed
#define eG1 occurance_pos
#define eF1 exponent_hash
#define eE1 exponent_list
#define eD1 CollectMulGroup(
#define eC1 source_set
#define eB1 nR3,TreeSet
#define eA1 pair<yW,TreeSet>
#define e91 .sep_list[
#define e81 )e9 yM2
#define e71 );range.multiply(
#define e61 CollectionSet<yW>
#define e51 (yU3 a=
#define e41 }break cS2
#define e31 iH))goto redo;
#define e21 e51 xW a-->0;)
#define e11 *const func)(
#define e01 eQ1 void
#define cZ1 grammar_rules[*r]
#define cY1 1)?(poly^(
#define cX1 );nR3.Rehash()
#define cW1 xE3 1
#define cV1 synth)
#define cU1 lP2);
#define cT1 produce_count
#define cS1 operator
#define cR1 TreeCountType<yW>
#define cQ1 AddParamMove iH);
#define cP1 retry_anyparams_3
#define cO1 retry_anyparams_2
#define cN1 needlist_cached_t
#define cM1 lJ 2 e63},{{1,
#define cL1 lJ 1 e63},{{1,
#define cK1 by_float_exponent
#define cJ1 lI3 nR3
#define cI1 new_exp
#define cH1 CodeTreeImmed(yW(
#define cG1 return BecomeZero;
#define cF1 return BecomeOne;
#define cE1 if(lO.size()<=n1)
#define cD1 addgroup
#define cC1 found_log2by
#define cB1 .GetParamCount()
#define cA1 nE==yL3)
#define c91 if(keep_powi
#define c81 ParsePowiMuli(
#define c71 eO3 532,{2,
#define c61 MakeTrue
#define c51 branch1_backup
#define c41 branch2_backup
#define c31 exponent_map
#define c21 plain_set
#define c11 rangehalf
#define c01 LightWeight(
#define yZ1 if(value
#define yY1 eQ1 static
#define yX1 cOr eJ3
#define yW1 CodeTreeImmed<yW>(
#define yV1 .l01 a-->0;)if(
#define yU1 cB1==
#define yT1 should_regenerate=true;
#define yS1 should_regenerate,
#define yR1 (long double)
#define yQ1 tF3&&i->first==
#define yP1 Collection
#define yO1 RelationshipResult
#define yN1 Subdivide_Combine(
#define yM1 long value
#define yL1 cM nY3 StackMax
#define yK1 ;flipped=!flipped;}
#define yJ1 tM3 yZ
#define yI1 rhs yJ1 hash1
#define yH1 for(yU3 a yO
#define yG1 best_sep_factor
#define yF1 tree cT2
#define yE1 needlist_cached
#define yD1 inline unsigned
#define yC1 opcode,bool pad
#define yB1 eQ1 l11
#define yA1 iY1 a);}
#define y91 MakesInteger(
#define y81 best_sep_cost
#define y71 MultiplicationRange
#define y61 pihalf_limits
#define y51 yN 2 c63;lD
#define y41 n_stacked
#define y31 AnyParams_Rec
#define y21 l63 FPoptimizer_Optimize
#define y11 ):e5(),std::vector<
#define y01 continue;
#define xZ1 PositionalParams,0}
#define xY1 always_sincostan
#define xX1 Recheck_RefCount_Div
#define xW1 Recheck_RefCount_Mul
#define xV1 mulgroup;mulgroup cC
#define xU1 MultiplyAndMakeLong(
#define xT1 lA2);
#define xS1 if(synth.FindAndDup(
#define xR1 SynthesizeParam(
#define xQ1 grammar_func
#define xP1 252415,{2,24830,
#define xO1 Modulo_Radians},
#define xN1 ,cIf,l0 3,
#define xM1 NeedList
#define xL1 ;l13<<
#define xK1 cS2 SubFunction:{
#define xJ1 )lT3.second
#define xI1 for e51 l01 a
#define xH1 ,typename xA::
#define xG1 ;e22
#define xF1 .IsImmed()
#define xE1 .data.subfunc_opcode
#define xD1 ;return
#define xC1 const std::vector<
#define xB1 matched_params
#define xA1 xD1 true;}
#define x91 Become(value l7 0));
#define x81 PositionType
#define x71 CollectionResult
#define x61 eQ1 bool
#define x51 const_offset
#define x41 inline TriTruthValue
#define x31 stacktop_desired
#define x21 SetStackTop(
#define x11 FPoptimizer_ByteCode
#define x01 xB leaf2 l7
#define nZ1 return lI
#define nY1 cond_type
#define nX1 Recheck_RefCount_RDiv
#define nW1 cMul tV2 tmp.
#define nV1 SwapLastTwoInStack();
#define nU1 DataP slot_holder(xZ[
#define nT1 fPExponentIsTooLarge(
#define nS1 CollectMulGroup_Item(
#define nR1 nN x21 xD-1);
#define nQ1 CalculateResultBoundaries
#define nP1 =nQ1 iH l7
#define nO1 );if(covers_plus1
#define nN1 nO yW(-t93
#define nM1 covers_full_cycle
#define nL1 AssembleSequence(
#define nK1 252180,{2,281854,
#define nJ1 .GetImmed()
#define nI1 ;eQ1
#define nH1 <<std::dec<<")";}
#define nG1 }cP MakeNotP1,l5::
#define nF1 }cP MakeNotP0,l5::
#define nE1 }cP MakeNotNotP1,l5::
#define nD1 }cP MakeNotNotP0,l5::
#define nC1 :tE3 i61
#define nB1 !=xE)if(TestCase(
#define nA1 &&IsLogicalValue(
#define n91 ,c83 165888,{2,
#define n81 ByteCodeSynth<yW>
#define n71 std::pair<T1,T2>&
#define n61 );tree t11
#define n51 t63 yN 2,cMul);lD
#define n41 eT1 typename
#define n31 has_good_balance_found
#define n21 n_occurrences
#define n11 found_log2_on_exponent
#define n01 covers_minus1
#define lZ1 cC cLog);n23 cMul)
#define lY1 xL2 nJ1;
#define lX1 needs_resynth
#define lW1 immed_product
#define lV1 ,2,1)nR if(found[data.
#define lU1 ParamSpec_Extract<yW>(
#define lT1 xX2 bitmask&
#define lS1 cT 523510,{2,
#define lR1 l1 x02{3,
#define lQ1 l1 eF3
#define lP1 Sign_Positive
#define lO1 SetParamMove(
#define lN1 Suboptimal
#define lM1 changed_if
#define lL1 n_as_tanh_param
#define lK1 MatchResultType
#define lJ1 needs_sincos
#define lI1 resulting_exponent
#define lH1 Unknown:default:;}
#define lG1 GetParam(a)
#define lF1 inverse_nominator]
#define lE1 cH1(
#define lD1 SetParams(iG1));
#define lC1 GetPositivityInfo iH)!=
#define lB1 AddFunctionOpcode_Integer(
#define lA1 o<<"("<<std::hex<<data.
#define l91 (val);else*this=model;}
#define l81 void FunctionParserBase<
#define l71 changed_exponent
#define l61 inverse_denominator
#define l51 retry_positionalparams_2
#define l41 PlanNtimesCache(
#define l31 AddFunctionOpcode_Float(
#define l21 FPoptimizer_Grammar
#define l11 static inline xA
#define l01 GetParamCount();
#define iZ IfBalanceGood(
#define iY n_as_tan_param
#define iX opposite=
#define iW AddOperation(cInv,1,1)nR}
#define iV cN2 ImmedHashGenerator
#define iU 512,{2,400412,
#define iT CopyOnWrite();
#define iS recursioncount
#define iR ParamSpec_SubFunctionData
#define iQ ]);xF3
#define iP eY2){synth.c2
#define iO NewHash
#define iN PositionalParams_Rec
#define iM }inline
#define iL tree.iY1 a
#define iK (unsigned
#define iJ ;for iK a=0;a<y2;++a)
#define iI )xL1 l53 DumpHashes
#define iH (tree
#define iG DumpTreeWithIndent(*this);
#define iF int_exponent_t
#define iE switch(type iK2 cond_or:
#define iD eT1 unsigned Compare>
#define iC lJ 2 nY2 1,
#define iB ByteCode
#define iA edited_powgroup
#define i9 =true;nC2!cG3
#define i8 has_unknown_max
#define i7 has_unknown_min
#define i6 synthed_tree
#define i5 SelectedParams,0 nY2
#define i4 collections
#define i3 cache
#define i2 ,{ProduceNewTree,
#define i1 y3 AnyParams,
#define i0 .push_back(
#define tZ goto ReplaceTreeWithOne;case
#define tY x53,std::ostream&o
#define tX 1.5)*fp_const_pi<yW>()
#define tW static const lK3
#define tV !=xE)return if_always[
#define tU cK1.data
#define tT iP2 x12(
#define tS needs_sinhcosh
#define tR cAdd l3 0,
#define tQ eQ1 n8
#define tP nU2,l5::
#define tO ].relationship
#define tN lC2 p0 i61>=yW(0.0))
#define tM p0=nQ1(
#define tL ,iB,IP,limit,y0,stack);
#define tK fphash_value_t(iI1)
#define tJ 408945,{2,24921,
#define tI unsigned opcode)
#define tH tZ1 n23 cZ3 nE);tree.
#define tG )xD1 m cS2
#define tF ;lO1 0,nW2;iY1 1);
#define tE [n1].first=true;lO[n1].second
#define tD l21::Grammar*
#define tC powgroup l7
#define tB GetLogicalValue iH l7
#define tA eY2&&found[data.
#define t9 Never}cP xR2 Never}}
#define t8 has_mulgroups_remaining
#define t7 by_exponent
#define t6 Rehash();i52 i0
#define t5 }if tG3 nJ1==yW(
#define t4 l63 FPoptimizer_CodeTree
#define t3 n_as_sinh_param
#define t2 n_as_cosh_param
#define t1 is_signed
#define t0 iG1));t21 Rehash();
#define eZ best_factor
#define eY RootPowerTable<yW>::RootPowers[
#define eX result_positivity
#define eW biggest_minimum
#define eV MatchPositionSpec_AnyParams<yW>
#define eU const iR
#define eT lA 0x4},{{
#define eS cond_tree
#define eR else_tree
#define eQ then_tree
#define eP sequencing
#define eO string FP_GetOpcodeName(
#define eN if_stack
#define eM (a);bool needs_cow=GetRefCount()>1;
#define eL relationships
#define eK .max.set(fp_ceil tG
#define eJ n_as_sin_param
#define eI n_as_cos_param
#define eH PowiResolver::
#define eG ];};extern"C"{
#define eF ParamSpec_NumConstant<yW>
#define eE .BalanceGood
#define eD {if(needs_cow){iT goto
#define eC valueType
#define eB back().endif_location
#define eA fphash_value_t key
#define e9 ;n12
#define e8 nP1 a));if(
#define e7 n12 mul);
#define e6 130,1,
#define e5 MatchPositionSpecBase
#define e4 iP2 CodeTree(
#define e3 smallest_maximum
#define e2 factor_needs_rehashing
#define e1 MatchPositionSpecBaseP
#define e0 goto ReplaceTreeWithParam0;
#define cZ {AdoptChildrenWithSameOpcode iH);
#define cY 79,119,120,124,125,130,131,134,135,
#define cX tY=l13
#define cW 28,29,30,31,32,33,34,35,36,
#define cV :goto ReplaceTreeWithZero;case
#define cU ;n23 nO3 nE t02}
#define cT ,l2 2,2,
#define cS 2,y02{2,401798,
#define cR 165,174,175,176,185,186,198,202,210,214,222,234,236,239,240,241,242,243,246,247,248,249,252,253,254,255,1,2}};}cN2
#define cQ unsigned c;unsigned char l[
#define cP ,{l5::
#define cO otherhalf
#define cN cMul l3 0,
#define cM StackState
#define cL CalculatePowiFactorCost(
#define cK ::map<fphash_t,std::set<std::string> >
#define cJ .AddParam(
#define cI n12 comp.c21[a].value);
#define cH T1,typename T2>inline iC2()(
#define cG m.min.
#define cF +=fp_const_twopi<yW>();
#define cE fp_sin(min),fp_sin(max))
#define cD fp_const_twopi t63;if(
#define cC .lJ2
#define cB =fp_cosh cA2);m xN=fp_cosh(m xN);
#define cA has_nonlogical_values
#define c9 from_logical_context)
#define c8 for e51 xY.l01 a-->0;)
#define c7 POWI_CACHE_SIZE
#define c6 PACKED_GRAMMAR_ATTRIBUTE;
#define c5 ++IP;y01}if(yC3 yB3.
#define c4 .FoundChild
#define c3 BalanceResultType
#define c2 DoDup(found[data.
#define c1 cIf,eG3
#define c0 nT3(0),Opcode(
#define yZ {return
#define yY l4 2,1,
#define yX const yZ data->
#define yW Value_t
#define yV typename cR1::xN3
#define yU {static void MakeHash(nD fphash_t&iO,
#define yT yS&&p0 xN<=fp_const_negativezero t63
#define yS .n53
#define yR iH)xL1"\n";
#define yQ ComparisonSetBase::
#define yP .Rehash(n61
#define yO =0;a<xL3.l01++a)if(
#define yN ;sim.Eat(
#define yM xF3 GetOpcode(),
#define yL e22 l01++a){if(
#define yK lU1 nP.param_list,
#define yJ MatchPositionSpec_AnyWhere
#define yI xZ3.data.match_type==
#define yH void OutFloatHex(std::ostream&o,
#define yG AddParam(CodeTreeImmed(
#define yF MatchPositionSpec_PositionalParams<yW>
#define yE c83 122999,{2,137349,
#define yD 0));tmp cC cInv);tmp t11 tmp2)xD1
#define yC c83 142455,{2,141449,
#define yB c83 522359,{2,24713,
#define yA AssembleSequence_Subdivide(
#define y9 ]=lP2|unsigned(
#define y8 ;lM1 tZ1 n23 op1);tree.
#define y7 },{{2,
#define y6 paramholder_matches
#define y5 )return false;
#define y4 (lS3 r=range.first;r!=range.second;++r){
#define y3 ,cAdd,
#define y2 nP y52
#define y1 branch2
#define y0 factor_stack_base
#define xZ data->yV3
#define xY branch1
#define xX MatchInfo<yW>&
#define xW tree.l01
#define xV const SequenceOpCode<yW>
#define xU ,yW(-1)))eD
#define xT AddConst(
#define xS {lL2.erase(i);y01}
#define xR FPOPT_autoptr
#define xQ +=cG3 xD1 cG3;}eQ1 inline yW
#define xP int_exponent
#define xO i42.IsImmed(
#define xN .t43
#define xM <yW>model=c11 t63{if(known
#define xL GetStackTop()-
#define xK e02 nJ1
#define xJ if(fp_nequal(tmp,yW(0)nJ yW(1)/tmp);n6}lD
#define xI newnode
#define xH ParamSpec_SubFunction
#define xG ParamSpec_ParamHolder
#define xF has_highlevel_opcodes
#define xE Unchanged
#define xD StackTop
#define xC best_selected_sep
#define xB .IsIdenticalTo(
#define xA CodeTree<yW>
#define x9 ->Recalculate_Hash_NoRecursion();}
#define x8 l01++a)if(ApplyGrammar(tC2,xL2,
#define x7 position
#define x6 l0 1,
#define x5 e22 xW++a){lK3
#define x4 std::vector<CodeTree>
#define x3 SetParam(0,nO3 l7 0));xA p1;p1 cC
#define x2 ,tree)){tree.FixIncompleteHashes();}
#define x1 TestImmedConstraints iS3 tP1,tree)y5
#define x0 SwapLastTwoInStack()yN
#define nZ );c91){sim cV2 cInv xC2 x82-1)y51
#define nY lJ 1 nY2
#define nX paramholder_index
#define nW tW1 case
#define nV occurance_counts
#define nU ,cMul l3 2,1,
#define nT >p e8 p.
#define nS -->0;){iK1 powgroup=lG1;if(powgroup
#define nR ;synth.StackTopIs(*this)xD1;}
#define nQ const FPoptimizer_CodeTree::xA&tree
#define nP model_tree
#define nO return lK3(
#define nN ){using l63 FUNCTIONPARSERTYPES;
#define nM t01&l32
#define nL AddParam iH l7
#define nK ConstantFolding_LogicCommon iH,yQ
#define nJ )){tree.ReplaceWithImmed(
#define nI n41 Ref>inline void xR<Ref>::
#define nH AnyParams,1 nY2
#define nG ,eJ1 e93
#define nF ):data(new x12<yW>(
#define nE .GetOpcode()
#define nD FUNCTIONPARSERTYPES::
#define nC ,cAdd l3 2,1,
#define nB b;}};eT1>cN2 Comp<nD
#define nA iI1(),yV3(),Hash(),Depth(1),t41 0){}
#define n9 SynthesizeByteCode(cV1;
#define n8 x12<yW>::x12(
#define n7 {xA tmp,tmp2;tmp2 cC
#define n6 goto do_return;}
#define n5 GetIntegerInfo iH l7 0))==IsAlways)e0
#define n4 eR1 cGreater>(yW(
#define n3 while(ApplyGrammar(n92 Grammar&)
#define n2 DumpParams<yW>(iC3.param_list,iC3 y52,o);
#define n1 restholder_index
#define n0 i42 nJ1
#define lZ :if(ParamComparer<yW>()(yV3[1],yV3[0])){std::swap(yV3[0],yV3[1]);Opcode=
#define lY <typename yW>
#define lX xA nR3;nR3 x03 nR3 cJ
#define lW <yW>tmp;tmp cC cPow tV2 tmp.yG yW(
#define lV nZ2,0x0},
#define lU n61 lM1)xA1
#define lT GroupFunction,0},lV{{
#define lS iO3 e02.IsImmed(nJ
#define lR ;tree t11 pow l7 1));pow.iY1 1);pow tZ1 tree.lO1 0,pow);goto NowWeAreMulGroup;}
#define lQ .hash1|=key;fphash_value_t crc=(key>>10)|(key<<(64-10))cU2((~fphash_value_t(crc))*3)^1234567;}};
#define lP ,yW(1)/yW(
#define lO restholder_matches
#define lN lM1;lM1 iP1 lM1.AddParamMove iH l7 0));lM1 cJ xY l7
#define lM eQ1 xA::CodeTree(
#define lL i21 0,i42 l7 0 lE3 CodeTreeImmed(
#define lK cMul yG3
#define lJ cMul,AnyParams,
#define lI nQ1(tmp)cS2
#define lH :xJ3 comp.AddRelationship(atree l7 0),atree l7 1),yQ
#define lG ){xA
#define lF ,cPow,lR3
#define lE typename yW>inline iC2()n92 yW&nU3 yW&b)yZ a
#define lD break;case
#define lC {lK3 m nP1 0));
#define lB e01 xA::
#define lA xZ1,0,
#define l9 l1 0x0 y7
#define l8 ?0:1));xA lM1;lM1 iP1 lM1.SetParamsMove iH.iG1));lM1 tZ1 n23
#define l7 .GetParam(
#define l6 SelectedParams,0},0,0x0 y7
#define l5 RangeComparisonData
#define l4 xZ1}i2
#define l3 ,AnyParams,0}},{ReplaceParams,
#define l2 xZ1},{ReplaceParams,
#define l1 cMul,SelectedParams,0},0,
#define l0 lA x02{
#ifdef _MSC_VER
typedef
unsigned
int
iF1;
#else
#include <stdint.h>
typedef
uint_least32_t
iF1;
#endif
l63
crc32{enum{startvalue=0xFFFFFFFFUL,poly=0xEDB88320UL}
;eT1
iF1
crc>cN2
b8{enum{b1=(crc&cY1
crc
x22
crc>>1),b2=(b1&cY1
b1
x22
b1>>1),b3=(b2&cY1
b2
x22
b2>>1),b4=(b3&cY1
b3
x22
b3>>1),b5=(b4&cY1
b4
x22
b4>>1),b6=(b5&cY1
b5
x22
b5>>1),b7=(b6&cY1
b6
x22
b6>>1),res=(b7&cY1
b7
x22
b7>>1)}
;}
;inline
iF1
update(iF1
crc
cQ2
b){
#define B4(n) b8<n eW2 n+1 eW2 n+2 eW2 n+3>::res
#define R(n) B4(n),B4(n+4),B4(n+8),B4(n+12)
static
const
iF1
table[256]={R(0x00),R(0x10),R(0x20),R(0x30),R(0x40),R(0x50),R(0x60),R(0x70),R(0x80),R(0x90),R(0xA0),R(0xB0),R(0xC0),R(0xD0),R(0xE0),R(0xF0)}
;
#undef R
#undef B4
return((crc>>8))^table[(crc^b)&0xFF];iM
iF1
calc_upd(iF1
c,const
unsigned
char*buf,yU3
size){iF1
value=c;for(yU3
p=0;p<size;++p)value=update(value,buf[p])xD1
value;iM
iF1
calc
n92
unsigned
char*buf,yU3
size)yZ
calc_upd(startvalue,buf,size);}
}
#ifndef FPOptimizerAutoPtrHH
#define FPOptimizerAutoPtrHH
n41
Ref>class
xR{cI3
xR():p(0){}
xR(Ref*b):p(b){nS3}
xR
n92
xR&b):p(b.p){nS3
iM
Ref&cS1*(yJ1*p;iM
Ref*cS1->(yJ1
p;}
xR&cS1=(Ref*b){Set(b)xD1*this;}
xR&cS1=n92
xR&b){Set(b.p)xD1*this;}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
xR(xR&&b):p(b.p){b.p=0;}
xR&cS1=(xR&&b){if(p!=b.p){cM2;p=b.p;b.p=0;tD3*this;}
#endif
~xR(){cM2
cN3
UnsafeSetP(Ref*newp){p=newp
cN3
swap(xR<Ref>&b){Ref*tmp=p;p=b.p;b.p=tmp;}
private:inline
static
void
Have(Ref*p2);inline
void
cM2;inline
void
nS3
inline
void
Set(Ref*p2);private:Ref*p;}
;nI
cM2{if(!p)return;p->nT3-=1;if(!p->nT3)delete
p;}
nI
Have(Ref*p2){if(p2)++(p2->nT3);}
nI
Birth(){Have(p);}
nI
Set(Ref*p2){Have(p2);cM2;p=p2;}
#endif
#include <utility>
cN2
Compare2ndRev{n41
T>inline
iC2()n92
T&nU3
T&b
yJ1
a.second>b.second;}
}
;cN2
Compare1st{n41
cH
const
n71
nU3
n71
b
yJ1
a.first<b.first;}
n41
cH
const
n71
a,T1
b
yJ1
a.first<b;}
n41
cH
T1
nU3
n71
b
yJ1
a<b.first;}
}
;
#ifndef FPoptimizerHashHH
#define FPoptimizerHashHH
#ifdef _MSC_VER
typedef
unsigned
long
long
fphash_value_t;
#define FPHASH_CONST(x) x##ULL
#else
#include <stdint.h>
typedef
uint_fast64_t
fphash_value_t;
#define FPHASH_CONST(x) x##ULL
#endif
l63
FUNCTIONPARSERTYPES{cN2
fphash_t{fphash_value_t
hash1,hash2;fphash_t():hash1(0),hash2(0){}
fphash_t
n92
fphash_value_t&nU3
fphash_value_t&b):hash1(a),hash2(b){}
iC2==n92
fphash_t&yI1==cO2&&hash2==cP2
iC2!=n92
fphash_t&yI1!=cO2||hash2!=cP2
iC2<n92
fphash_t&yI1!=cO2?hash1<cO2:hash2<cP2}
;}
#endif
#ifndef FPOptimizer_CodeTreeHH
#define FPOptimizer_CodeTreeHH
#ifdef FP_SUPPORT_OPTIMIZER
#include <vector>
#include <utility>
l63
l21{cN2
Grammar;}
l63
x11{eQ1
class
ByteCodeSynth;}
t4{eQ1
class
CodeTree
nI1
cN2
x12
nI1
class
CodeTree{typedef
xR<x12<yW> >DataP;DataP
data;cI3
CodeTree();~CodeTree();cN2
OpcodeTag{}
;e4
cR2
o,OpcodeTag);cN2
FuncOpcodeTag{}
;e4
cR2
o
cQ2
f,FuncOpcodeTag);cN2
nV3{}
;e4
const
yW&v,nV3);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
e4
yW&&v,nV3);
#endif
cN2
VarTag{}
;e4
unsigned
varno,VarTag);cN2
CloneTag{}
;e4
l73
b,CloneTag);void
GenerateFrom
n92
typename
FunctionParserBase<yW>::Data&data,bool
keep_powi=false);void
GenerateFrom
n92
typename
FunctionParserBase<yW>::Data&data,const
x4&nV2,bool
keep_powi=false);void
SynthesizeByteCode(std::vector<unsigned>&yS3,std::vector<yW>&immed,yU3&stacktop_max);void
SynthesizeByteCode(x11::n81&synth,bool
MustPopTemps=true
tM3;yU3
SynthCommonSubExpressions(x11::n81&cV1
const;void
SetParams
n92
x4&nW3
SetParamsMove(x4&eS1
CodeTree
GetUniqueRef();
#ifdef __GXX_EXPERIMENTAL_CXX0X__
void
SetParams(x4&&eS1
#endif
void
SetParam(yU3
which,l73
b);void
lO1
yU3
which,nT2
b);void
AddParam
n92
nT2
param);void
n12
nT2
param);void
AddParams
n92
x4&nW3
AddParamsMove(x4&nW3
AddParamsMove(x4&l32,yU3
l42);void
iY1
yU3
index);void
DelParams();void
Become
n92
nT2
b);inline
yU3
GetParamCount(yJ1
iG1).size();iM
nT2
GetParam(y73)yZ
iG1)[n];iM
l73
GetParam(y73
yJ1
iG1)[n];iM
void
lJ2
cR2
o)eK3
Opcode=o;iM
cR2
GetOpcode()yX
Opcode;iM
nD
fphash_t
GetHash()yX
Hash;iM
const
x4&iG1
yJ1
xZ;iM
x4&iG1)yZ
xZ;iM
yU3
GetDepth()yX
Depth;iM
const
yW&GetImmed()yX
Value;iM
unsigned
GetVar()yX
iH1
iM
unsigned
GetFuncNo()yX
iH1
iM
bool
IsDefined(yJ1
GetOpcode()!=nD
cNop;iM
bool
IsImmed(yJ1
GetOpcode()==nD
cImmed;iM
bool
IsVar(yJ1
GetOpcode()==nD
iN2;iM
unsigned
GetRefCount()yX
nT3
cN3
ReplaceWithImmed
n92
yW&i);void
Rehash(bool
constantfolding=true);void
Sort();inline
void
Mark_Incompletely_Hashed()eK3
Depth=0;iM
bool
Is_Incompletely_Hashed()yX
Depth==0;iM
const
tD
GetOptimizedUsing()yX
iJ1;iM
void
SetOptimizedUsing
n92
tD
g)eK3
iJ1=g;}
bool
RecreateInversionsAndNegations(bool
prefer_base2=false);void
FixIncompleteHashes();void
swap(nT2
b){data.swap(b.data);}
bool
IsIdenticalTo
n92
nT2
b
tM3;void
iT}
nI1
cN2
x12{int
nT3;cR2
Opcode;yW
Value;unsigned
iH1
t01
yV3;nD
fphash_t
Hash;yU3
Depth;const
tD
iJ1;x12();x12
n92
x12&b);tT
cR2
o);tT
cR2
o
cQ2
f);tT
const
yW&i);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
tT
yW&&i);x12(x12&&b);
#endif
bool
IsIdenticalTo
n92
x12&b
tM3;void
Sort();void
Recalculate_Hash_NoRecursion();private:void
cS1=n92
x12&b);}
nI1
l11
CodeTreeImmed
n92
yW&i)yZ
xA(i
xH1
nV3());}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
yB1
CodeTreeImmed(yW&&i)yZ
xA
c12
i)xH1
nV3());}
#endif
yB1
CodeTreeOp(cR2
opcode)yZ
xA(opcode
xH1
OpcodeTag());}
yB1
CodeTreeFuncOp(cR2
opcode
cQ2
f)yZ
xA(opcode,f
xH1
FuncOpcodeTag());}
yB1
CodeTreeVar
iK
varno)yZ
xA(varno
xH1
VarTag());}
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
e01
DumpHashes(cX)x43
lX3
cX)x43
DumpTreeWithIndent(cX,const
std::string&indent="\\"
);
#endif
}
#endif
#endif
#ifndef FPOPT_NAN_CONST
#include <iostream>
#define FPOPT_NAN_CONST (-1712345.25)
t4{eQ1
class
CodeTree;}
l63
l21{enum
ImmedConstraint_Value{ValueMask=0x07,Value_AnyNum=0x0,nG2=0x1,Value_OddInt=0x2,t51=0x3,Value_NonInteger=0x4,nA2=0x5
cO3
ImmedConstraint_Sign{SignMask=0x18,Sign_AnySign=0x00,lP1=0x08,eI1=0x10,Sign_NoIdea=0x18
cO3
ImmedConstraint_Oneness{OnenessMask=0x60,Oneness_Any=0x00,Oneness_One=0x20,Oneness_NotOne=0x40
cO3
ImmedConstraint_Constness{ConstnessMask=0x180,Constness_Any=0x00,nZ2=0x80,Constness_NotConst=0x100
cO3
Modulo_Mode{Modulo_None=0,Modulo_Radians=1
cO3
Situation_Flags{LogicalContextOnly=0x01,NotForIntegers=0x02,OnlyForIntegers=0x04
cO3
n22{NumConstant,ParamHolder,SubFunction
cO3
ParamMatchingType{PositionalParams,SelectedParams,AnyParams,GroupFunction
cO3
RuleType{ProduceNewTree,ReplaceParams}
;
#ifdef __GNUC__
# define PACKED_GRAMMAR_ATTRIBUTE __attribute__((packed))
#else
# define PACKED_GRAMMAR_ATTRIBUTE
#endif
typedef
std::pair<n22,const
void*>cW2
nI1
cW2
ParamSpec_Extract
iK
paramlist
cQ2
index)nI1
bool
ParamSpec_Compare
n92
void*nU3
void*b,n22
type);unsigned
ParamSpec_GetDepCode
n92
cW2&b);cN2
xG
cF3
index:8;unsigned
constraints:9;unsigned
depcode:15;}
c6
eQ1
cN2
ParamSpec_NumConstant{yW
constvalue;unsigned
modulo;c93
iR
cF3
param_count:2;unsigned
param_list:30;cR2
subfunc_opcode:8;ParamMatchingType
match_type:3;unsigned
n1:5;c93
xH{iR
data;unsigned
constraints:9;unsigned
depcode:7;c93
Rule{RuleType
ruletype:2;unsigned
situation_flags:3;unsigned
repl_param_count:2+11;unsigned
repl_param_list:30;iR
match_tree;c93
Grammar
cF3
rule_count;unsigned
char
rule_list[999
eG
extern
const
Rule
grammar_rules[];}
e01
DumpParam
n92
cW2&p,std::ostream&o=l13)x43
DumpParams
iK
paramlist
cQ2
count,std::ostream&o=l13);}
#endif
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#define CONSTANT_POS_INF HUGE_VAL
#define CONSTANT_NEG_INF (-HUGE_VAL)
l63
FUNCTIONPARSERTYPES{eQ1
inline
yW
fp_const_pihalf()yZ
fp_const_pi<yW>()*yW(0.5);}
eQ1
inline
yW
fp_const_twopi()cP3
fp_const_pi
tB3
fp_const_twoe()cP3
fp_const_e
tB3
fp_const_twoeinv()cP3
fp_const_einv
tB3
fp_const_negativezero(){
#ifdef FP_EPSILON
return-fp_epsilon<yW>();
#else
return
yW(-1e-14);
#endif
}
}
#ifdef FP_SUPPORT_OPTIMIZER
#include <vector>
#include <utility>
#include <iostream>
y21{using
l63
l21;using
t4;using
l63
FUNCTIONPARSERTYPES
nI1
class
MatchInfo{cI3
std::vector<std::pair<bool,t01> >lO;t01
y6
e52
unsigned>xB1;cI3
MatchInfo():lO(),y6(),xB1(){}
cI3
bool
SaveOrTestRestHolder
iK
n1,nX3
tG1){cE1{lO
nY3
n1+1);lO
tE=tG1
xA1
if(lO[n1].first==false){lO
tE=tG1
xA1
nX3
found=lO[n1].second;if(tG1.size()!=found.size()y5
e22
tG1
iT2
a)if(!tG1[a]xB
found[a])y5
tW1}
void
SaveRestHolder
iK
n1,t01&tG1){cE1
lO
nY3
n1+1);lO
tE.swap(tG1);}
bool
SaveOrTestParamHolder
iK
nX,iK1
x33){if(y6.size()<=nX){y6.x63
nX+1);y6
nY3
nX);y6
i0
x33)xA1
if(!y6[nX].lZ2){y6[nX]=x33
xA1
return
x33
xB
y6[nX]cM3
SaveMatchedParamIndex
iK
index){xB1
i0
index);}
iK1
GetParamHolderValueIfFound
iK
nX
tM3{static
const
xA
dummytree;if(y6.size()<=nX)return
dummytree
xD1
y6[nX];}
iK1
GetParamHolderValue
iK
nX
yJ1
y6[nX];}
bool
HasRestHolder
iK
n1
yJ1
lO.size()>n1&&lO[n1].first==true;}
nX3
GetRestHolderValues
iK
n1
tM3{static
xC1
xA>empty_result;cE1
return
empty_result
xD1
lO[n1].second;}
xC1
unsigned>&GetMatchedParamIndexes(yJ1
xB1
cN3
swap(xX
b){lO.swap(b.lO);y6.swap(b.y6);xB1.swap(b.xB1);}
xX
cS1=n92
xX
b){lO=b.lO;y6=b.y6;xB1=b.xB1
xD1*this;}
}
;class
e5;typedef
xR<e5>e1;class
e5{cI3
int
nT3;cI3
e5():nT3(0){}
virtual~e5(){}
}
;cN2
lK1{bool
found;e1
specs;lK1(bool
f):found(f),specs(){}
lK1(bool
f
lM3
s):found(f),specs(s){}
}
x43
SynthesizeRule
nD2
xA&tree,iQ3)nI1
lK1
TestParam
n92
cW2&y62
x53
lM3
iB2,iQ3)nI1
lK1
TestParams(eU&nP,x53
lM3
iB2,iQ3,bool
xZ2
nI1
bool
ApplyGrammar
n92
Grammar&tC2,FPoptimizer_CodeTree::xA&tree,bool
from_logical_context=false)x43
ApplyGrammars(FPoptimizer_CodeTree::xA&tree)nI1
bool
IsLogisticallyPlausibleParamsMatch(eU&c43,x53);}
l63
l21{e01
DumpMatch
nD2
nQ,const
FPoptimizer_Optimize::iQ3,bool
DidMatch,std::ostream&o=l13)x43
DumpMatch
nD2
nQ,const
FPoptimizer_Optimize::iQ3,l33
eT3,std::ostream&o=l13);}
#endif
#include <string>
l43
l21::n22
yC1=false);l43
cR2
yC1=false);
#include <string>
#include <sstream>
#include <assert.h>
#include <iostream>
using
l63
l21;using
l63
FUNCTIONPARSERTYPES;l43
l21::n22
yC1){
#if 1
l33
p=0;switch(opcode
iK2
NumConstant:p="NumConstant"
;lD
ParamHolder:p="ParamHolder"
;lD
SubFunction:p="SubFunction"
;break;}
std::ostringstream
tmp;assert(p);tmp<<p;if(pad)while(tmp.str().size()<12)tmp<<' 'xD1
tmp.str();
#else
std::ostringstream
tmp;tmp<<opcode;if(pad)while(tmp.str().size()<5)tmp<<' 'xD1
tmp.str();
#endif
}
l43
cR2
yC1){
#if 1
l33
p=0;switch(opcode
iK2
cAbs:p="cAbs"
;lD
cAcos:p="cAcos"
;lD
cAcosh:p="cAcosh"
;lD
cAsin:p="cAsin"
;lD
cAsinh:p="cAsinh"
;lD
cAtan:p="cAtan"
;lD
cAtan2:p="cAtan2"
;lD
cAtanh:p="cAtanh"
;lD
cCbrt:p="cCbrt"
;lD
cCeil:p="cCeil"
;lD
cCos:p="cCos"
;lD
cCosh:p="cCosh"
;lD
cCot:p="cCot"
;lD
cCsc:p="cCsc"
;lD
cEval:p="cEval"
;lD
cExp:p="cExp"
;lD
cExp2:p="cExp2"
;lD
cFloor:p="cFloor"
;lD
cHypot:p="cHypot"
;lD
cIf:p="cIf"
;lD
cInt:p="cInt"
;lD
cLog:p="cLog"
;lD
cLog2:p="cLog2"
;lD
cLog10:p="cLog10"
;lD
cMax:p="cMax"
;lD
cMin:p="cMin"
;lD
cPow:p="cPow"
;lD
cSec:p="cSec"
;lD
cSin:p="cSin"
;lD
cSinh:p="cSinh"
;lD
cSqrt:p="cSqrt"
;lD
cTan:p="cTan"
;lD
cTanh:p="cTanh"
;lD
cTrunc:p="cTrunc"
;lD
cImmed:p="cImmed"
;lD
cJump:p="cJump"
;lD
cNeg:p="cNeg"
;lD
cAdd:p="cAdd"
;lD
cSub:p="cSub"
;lD
cMul:p="cMul"
;lD
cDiv:p="cDiv"
;lD
cMod:p="cMod"
;lD
cEqual:p="cEqual"
;lD
t81:p="cNEqual"
;lD
cLess:p="cLess"
;lD
cLessOrEq:p="cLessOrEq"
;lD
cGreater:p="cGreater"
;lD
cGreaterOrEq:p="cGreaterOrEq"
;lD
cNot:p="cNot"
;lD
cAnd:p="cAnd"
;lD
cOr:p="cOr"
;lD
cDeg:p="cDeg"
;lD
cRad:p="cRad"
;lD
cFCall:p="cFCall"
;lD
cPCall:p="cPCall"
;break;
#ifdef FP_SUPPORT_OPTIMIZER
case
cFetch:p="cFetch"
;lD
cPopNMov:p="cPopNMov"
;lD
yQ3:p="cLog2by"
;lD
cNop:p="cNop"
;break;
#endif
case
cSinCos:p="cSinCos"
;lD
cSinhCosh:p="cSinhCosh"
;lD
yL3:p="cAbsNot"
;lD
cAbsNotNot:p="cAbsNotNot"
;lD
cAbsAnd:p="cAbsAnd"
;lD
cAbsOr:p="cAbsOr"
;lD
cAbsIf:p="cAbsIf"
;lD
cDup:p="cDup"
;lD
cInv:p="cInv"
;lD
cSqr:p="cSqr"
;lD
cRDiv:p="cRDiv"
;lD
cRSub:p="cRSub"
;lD
cNotNot:p="cNotNot"
;lD
cRSqrt:p="cRSqrt"
;lD
iN2:p="VarBegin"
;break;}
std::ostringstream
tmp;assert(p);tmp<<p;if(pad)while(tmp.str().size()<12)tmp<<' 'xD1
tmp.str();
#else
std::ostringstream
tmp;tmp<<opcode;if(pad)while(tmp.str().size()<5)tmp<<' 'xD1
tmp.str();
#endif
}
#ifdef FP_SUPPORT_OPTIMIZER
#include <vector>
#include <utility>
#ifndef FP_GENERATING_POWI_TABLE
enum{MAX_POWI_BYTECODE_LENGTH=20}
;
#else
enum{MAX_POWI_BYTECODE_LENGTH=999}
;
#endif
enum{MAX_MULI_BYTECODE_LENGTH=3}
;l63
x11{eQ1
class
ByteCodeSynth{cI3
ByteCodeSynth():iB(),Immed(),cM(),xD(0),StackMax(0){iB.x63
64);Immed.x63
8);cM.x63
16
cM3
Pull(std::vector<unsigned>&bc,std::vector<yW>&imm,yU3&StackTop_max){for
iK
a=0;a<iB
iT2
a){iB[a]&=~lP2;}
iB.swap(bc);Immed.swap(imm);StackTop_max=StackMax;}
yU3
GetByteCodeSize(yJ1
iB.size();}
yU3
GetStackTop(yJ1
xD
cN3
PushVar
iK
varno){iB
i0
varno);eX2}
void
PushImmed(yW
immed
nN
iB
i0
cImmed);Immed
i0
immed);eX2}
void
StackTopIs(nQ,int
offset=0){if((int)xD>offset){cM[xD
lQ2
first=true;cM[xD
lQ2
second=tree;}
}
bool
IsStackTop(nQ,int
offset=0
yJ1(int)xD>offset&&cM[xD
lQ2
first&&cM[xD
lQ2
second
xB
tree);iM
void
EatNParams
iK
cX2){xD-=cX2
cN3
ProducedNParams
iK
cT1){x21
xD+cT1
cM3
AddOperation
iK
opcode
cQ2
cX2
cQ2
cT1=1){EatNParams(cX2);using
l63
FUNCTIONPARSERTYPES;AddFunctionOpcode(opcode);ProducedNParams(cT1
cM3
DoPopNMov(yU3
cY2,yU3
srcpos
nN
iB
i0
cPopNMov);iL2
cY2);iL2
srcpos);x21
srcpos+1);cM[cY2]=cM[srcpos];x21
cY2+1
cM3
DoDup(yU3
x73
nN
if(x73==xD-1){iB
i0
cDup);}
else{iB
i0
cFetch);iL2
x73);}
eX2
cM[xD-1]=cM[x73];}
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
eT1
int>void
Dump(){std::ostream&o=l13;o<<"Stack state now("
<<xD<<"):\n"
xG1
xD;++a){o<<a<<": "
;if(cM[a].first){nQ=cM[a].second;o<<'['<<std::hex<<(void*)(&tree.iG1))<<std::dec<<','<<tree.GetRefCount()<<']';DumpTree
iH,o);}
else
o<<"?"
;o<<"\n"
;}
o<<std::flush;}
#endif
yU3
cQ3(nQ
tM3{for
e51
xD;a-->0;)if(cM[a].first&&cM[a].second
xB
tree
lN3
a
xD1
eY2;}
bool
Find(nQ
yJ1
cQ3
iH)!=eY2;}
bool
FindAndDup(nQ){yU3
pos=cQ3
iH);if(pos!=eY2){
#ifdef DEBUG_SUBSTITUTIONS
l13<<iM3"duplicate at ["
<<pos<<"]: "
;DumpTree
iH)xL1" -- issuing cDup or cFetch\n"
;
#endif
DoDup(pos)xA1
return
e03
cN2
IfData{yU3
ofs;}
;void
SynthIfStep1
t83,cR2
op
nR1
tA3=iB.size(i43
op
i43
cU1
iB
i0
cU1}
void
SynthIfStep2
t83
nR1
iB
lR2
eV1+2);iB
lR2+2
eW1
tA3=iB.size(i43
cJump
i43
cU1
iB
i0
cU1}
void
SynthIfStep3
t83
nR1
iB.back()|=lP2;iB
lR2
eV1-1);iB
lR2+2
eW1
x21
xD+1)xG1
tA3;++a){if(iB[a]==cJump&&iB[a+1]==(lP2|(tA3-1))){iB[a
eV1-1);iB[a+2
eW1}
switch(iB[a]iK2
cAbsIf:case
cIf:case
cJump:case
cPopNMov:a+=2;lD
cFCall:case
cPCall:case
cFetch:a+=1;break;default:y93}
protected:void
x21
yU3
value){xD=value;if(xD
eZ2{StackMax=xD;yL1);}
}
void
l31
tI;void
lB1
tI;void
AddFunctionOpcode(tI;private:std::vector<unsigned>iB
e52
yW>Immed
e52
std::pair<bool,FPoptimizer_CodeTree::xA> >cM;yU3
xD;yU3
StackMax;}
nI1
cN2
SequenceOpCode
nI1
cN2
eX1{static
xV
AddSequence;static
xV
MulSequence;}
x43
nL1
long
count,xV&eP,n81&cV1;}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63
x11{eQ1
cN2
SequenceOpCode{yW
basevalue;unsigned
op_flip;unsigned
op_normal,op_normal_flip;unsigned
op_inverse,op_inverse_flip;}
nI1
xV
eX1<yW>::AddSequence={yW(0),cNeg
y3
cAdd,cSub,cRSub}
nI1
xV
eX1<yW>::MulSequence={yW(1),cInv,cMul,cMul,cDiv,cRDiv}
x43
n81::l31
tI{int
mStackPtr=0;
#define incStackPtr() do{if(xD+2 eZ2 yL1=xD+2);}while(0)
#define findName(a,b,c) "var"
#define TryCompilePowi(o) false
#define mData this
#define mByteCode iB
#define mImmed Immed
# define FP_FLOAT_VERSION 1
# include "fp_opcode_add.inc"
# undef FP_FLOAT_VERSION
#undef mImmed
#undef mByteCode
#undef mData
#undef TryCompilePowi
#undef incStackPtr
}
e01
n81::lB1
tI{int
mStackPtr=0;
#define incStackPtr() do{if(xD+2 eZ2 yL1=xD+2);}while(0)
#define findName(a,b,c) "var"
#define TryCompilePowi(o) false
#define mData this
#define mByteCode iB
#define mImmed Immed
# define FP_FLOAT_VERSION 0
# include "fp_opcode_add.inc"
# undef FP_FLOAT_VERSION
#undef mImmed
#undef mByteCode
#undef mData
#undef TryCompilePowi
#undef incStackPtr
}
e01
n81::AddFunctionOpcode(tI{if(IsIntType<yW>::cG3)lB1
opcode);else
l31
opcode);}
}
using
l63
x11;
#define POWI_TABLE_SIZE 256
#define POWI_WINDOW_SIZE 3
l63
x11{
#ifndef FP_GENERATING_POWI_TABLE
extern
const
unsigned
char
powi_table[POWI_TABLE_SIZE];const
#endif
unsigned
char
powi_table[POWI_TABLE_SIZE]={0,1,1
x32
2,1,4
x32
4,1,2,131,8
x32
4
x32
8,yW3
131,4,1,15,1,16
x32
4,1,2,131,8
x32
4,yW3
1,16,1,25,131,4,1,27,5,8,3,2,1,30,1,31,3,32
x32
4
x32
8,1,2,131,4,1,39,1,16,137,2,1,4,yW3
131,8,1,45,135,4,31,2,5,32,1,2,131,50,1,51,1,8,3,2,1,54,1,55,3,16,1,57,133,4,137,2,135,60,1,61,3,62,133,63,1,tH1
131,tH1
139,iQ1
e6
30,1,130,137,2,31,iQ1
e6
e6
130,yW3
1,e6
e6
2,1,130,133,tH1
61,130,133,62,139,130,137,e6
iQ1
e6
e6
tH1
131,e6
e6
130,131,2,133,iQ1
130,141,e6
130,yW3
1,e6
5,135,e6
iQ1
e6
iQ1
130,133,130,141,130,131,e6
e6
2,131}
;}
static
nG3
c7=256;
#define FPO(x)
l63{class
iR1{private:int
i3[c7];int
tI1[c7];cI3
iR1():i3(),tI1(){i3[1]=1;}
bool
Plan_Add(yM1,int
count){yZ1>=c7
y5
tI1[t12+=count
xD1
i3[t12!=0
cN3
l83
yM1){yZ1<c7)i3[t12=1
cN3
Start(yU3
value1_pos){for(int
n=2;n<c7;++n)i3[n]=-1;Remember(1,value1_pos);DumpContents();}
int
Find(yM1
tM3{yZ1<c7){if(i3[t12>=0){FPO(i93(iE3,"* I found %ld from cache (%u,%d)\n",value,(unsigned)cache[value],iA3 value]))xD1
i3[t12;}
tD3-1
cN3
Remember(yM1,yU3
iL3){yZ1>=c7)return;FPO(i93(iE3,"* Remembering that %ld can be found at %u (%d uses remain)\n",value,(unsigned)iL3,iA3 value]));i3[t12=(int)iL3
cN3
DumpContents
tL3
FPO(for(int a=1;a<POWI_CACHE_SIZE;++a)if(cache[a]>=0||iA3 a]>0){i93(iE3,"== cache: sp=%d, val=%d, needs=%d\n",cache[a],a,iA3 a]);})}
int
UseGetNeeded(yM1){yZ1>=0&&value<c7)return--tI1[t12
xD1
0;}
}
nI1
yU3
yA
long
count
tS3,xV&eP,n81&cV1
x43
yN1
yU3
apos,long
aval,yU3
bpos,long
bval
tS3
cQ2
cumulation_opcode
cQ2
cimulation_opcode_flip,n81&cV1;void
l41
yM1
tS3,int
need_count,int
iS=0){yZ1<1)return;
#ifdef FP_GENERATING_POWI_TABLE
if(iS>32)throw
false;
#endif
if(i3.Plan_Add(value,need_count
lN3;long
x83
1;yZ1<POWI_TABLE_SIZE){x83
powi_table[t12
e43
128){half&=127
e43
64)x83-e53
FPO(i93(iE3,"value=%ld, half=%ld, otherhalf=%ld\n",value,half,value/half));l41
half,i3,x93
i3.l83
half)xD1;}
nC2
half&64){x83-e53}
}
else
yZ1&1)x83
value&((1<<POWI_WINDOW_SIZE)-1);else
x83
value/2;long
cO=value-half;if(half>cO||half<0)std::swap(half,cO);FPO(i93(iE3,"value=%ld, half=%ld, otherhalf=%ld\n",value,half,otherhalf));if(half==cO){l41
half,i3,2,iS+1);}
else{l41
half,i3,x93
l41
cO>0?cO:-cO,i3,x93}
i3.l83
value);}
eQ1
yU3
yA
yM1
tS3,xV&eP,xC3
int
xB3=i3.Find(value);if(xB3>=0)yZ
xB3;}
long
x83
1;yZ1<POWI_TABLE_SIZE){x83
powi_table[t12
e43
128){half&=127
e43
64)x83-e53
FPO(i93(iE3,"* I want %ld, my plan is %ld * %ld\n",value,half,value/half));yU3
x42=yA
half
lS2
if(i3
xD2
half)>0||x42!=cW1){tJ1
x42);i3
t22
half,cW1);}
nL1
value/half
tT3
yU3
iL3=cW1;i3
t22
value,iL3);i3.DumpContents()xD1
iL3;}
nC2
half&64){x83-e53}
}
else
yZ1&1)x83
value&((1<<POWI_WINDOW_SIZE)-1);else
x83
value/2;long
cO=value-half;if(half>cO||half<0)std::swap(half,cO);FPO(i93(iE3,"* I want %ld, my plan is %ld + %ld\n",value,half,value-half));if(half==cO){yU3
x42=yA
half
lS2
yN1
x42,half,x42,half,i3,eP.op_normal,eP.op_normal_flip,cV1;}
else{long
part1=half;long
part2=cO>0?cO:-cO;yU3
part1_pos=yA
part1
lS2
yU3
part2_pos=yA
part2
lS2
FPO(i93(iE3,"Subdivide(%ld: %ld, %ld)\n",value,half,otherhalf));yN1
part1_pos,part1,part2_pos,part2,i3,cO>0?eP.op_normal:eP.op_inverse,cO>0?eP.op_normal_flip:eP.op_inverse_flip,cV1;}
yU3
iL3=cW1;i3
t22
value,iL3);i3.DumpContents()xD1
iL3;}
e01
yN1
yU3
apos,long
aval,yU3
bpos,long
bval
tS3
cQ2
cumulation_opcode
cQ2
cumulation_opcode_flip,xC3
int
a_needed=i3
xD2
aval);int
xD3=i3
xD2
bval);bool
flipped=false;
#define DUP_BOTH() do{if(apos<bpos){yU3 tmp=apos;apos=bpos;bpos=tmp yK1 FPO(i93(iE3,"-> "iJ3 iJ3"op\n",(unsigned)apos,(unsigned)bpos));tJ1 apos);tJ1 apos==bpos?cW1:bpos);}while(0)
#define DUP_ONE(p) do{FPO(i93(iE3,"-> "iJ3"op\n",(unsigned)p));tJ1 p);}while(0)
if(a_needed>0){if(xD3>0){lU2}
tC3(bpos!=cW1)lU2
else{n42
yK1}
}
nC2
xD3>0){if(apos!=cW1)lU2
else
DUP_ONE(bpos);}
tC3(apos==bpos&&apos==cW1)n42;nC2
apos==cW1&&bpos==xE3
2){FPO(i93(iE3,"-> op\n"))yK1
nC2
apos==xE3
2&&bpos==cW1)FPO(i93(iE3,"-> op\n"));nC2
apos==cW1)DUP_ONE(bpos);nC2
bpos==cW1){n42
yK1
else
lU2}
xF3
flipped?cumulation_opcode_flip:cumulation_opcode,2);}
e01
c01
long
count,xV&eP,xC3
while
yX3<256){int
x83
x11::powi_table[count]e43
128){half&=127;c01
half
tT3
count/=half;}
else
break;}
if
yX3==1)return;if(!yX3&1)){xF3
cSqr,1);c01
count/2
tT3}
else{tJ1
cW1);c01
count-1
tT3
xF3
cMul,2);}
}
}
l63
x11{e01
nL1
long
count,xV&eP,xC3
if
yX3==0)i31
eP.basevalue);else{bool
t32=false;if
yX3<0){t32=true;count=-count;}
if(false)c01
count
tT3
nC2
count>1){iR1
i3;l41
count,i3,1);yU3
x31
eZ3
GetStackTop();i3.Start(cW1);FPO(i93(iE3,"Calculating result for %ld...\n",count));yU3
x52=yA
count
lS2
yU3
n_excess=xE3
x31;if(n_excess>0||x52!=x31-1){synth.DoPopNMov(x31-1,x52);}
}
if(t32)xF3
eP.op_flip,1);}
}
}
#endif
#ifndef FPOptimizer_ValueRangeHH
#define FPOptimizer_ValueRangeHH
t4{l63
l93{iD
cN2
Comp{}
;eT1>cN2
Comp<nD
cLess>{eT1
lE<nB
cLessOrEq>{eT1
lE<=nB
cGreater>{eT1
lE>nB
cGreaterOrEq>{eT1
lE>=nB
cEqual>{eT1
lE==nB
t81>{eT1
lE!=b;}
}
;}
eQ1
cN2
c11{yW
val;bool
known;c11():val(),known(false){}
c11
xG3):val(v),known(true){iM
xH3
xG3){known=true;val=v;}
xH3(yW(e11
yW
iK3
i41
xH3(yW(e11
const
yW&iK3
i41
iD
void
set_if(yW
v,yW(e11
yW
iK3&&l93::Comp<Compare>()(val,v)i41
iD
void
set_if
xG3,yW(e11
const
yW&iK3&&l93::Comp<Compare>()(val,v)i41}
nI1
cN2
range{c11<yW>min,max;range():min(),max()yY3
yW
mi,yW
ma):min(mi),max(ma)yY3
bool,yW
ma):min(),max(ma)yY3
yW
mi,bool):min(mi),max(){}
void
set_abs();void
set_neg();}
nI1
bool
IsLogicalTrueValue
n92
lK3&p
xA3
nI1
bool
IsLogicalFalseValue
n92
lK3&p
xA3;}
#endif
#ifndef FPOptimizer_RangeEstimationHH
#define FPOptimizer_RangeEstimationHH
t4{enum
TriTruthValue{IsAlways,IsNever,Unknown}
nI1
lK3
nQ1
cR3
bool
IsLogicalValue
cR3
TriTruthValue
GetIntegerInfo
cR3
x41
GetEvennessInfo
iS1{if(!tree
xF1)return
Unknown;lE2=tree
nJ1;if(isEvenInteger(value
lO3
isOddInteger(value
lT2
eQ1
x41
GetPositivityInfo
iS1{lK3
p=nQ1
iH);if(p.min
iR2
p
i61>=yW(lO3
p
iS2<yW(lT2
eQ1
x41
GetLogicalValue(x53
xA3{lK3
p=nQ1
iH);if(IsLogicalTrueValue(p,abs
lO3
IsLogicalFalseValue(p,abs
lT2}
#endif
#ifndef FPOptimizer_ConstantFoldingHH
#define FPOptimizer_ConstantFoldingHH
t4{e01
ConstantFolding(xA&tree);}
#endif
l63{using
l63
FUNCTIONPARSERTYPES;using
t4;cN2
ComparisonSetBase{enum{cS3=0x1,Eq_Mask=0x2,Le_Mask=0x3,cT3=0x4,cU3=0x5,Ge_Mask=0x6}
;static
int
Swap_Mask(int
m)yZ(m&Eq_Mask)|((m&cS3)?cT3:0)|((m&cT3)?cS3:0);}
enum
yO1{Ok,BecomeZero,BecomeOne,lN1
cO3
n52{cond_or,iE2,iF2,iG2}
;}
nI1
cN2
ComparisonSet:public
ComparisonSetBase{cN2
t42{xA
a;xA
b;int
relationship;t42():a(),b(),relationship(){}
}
e52
t42>eL;cN2
Item{xA
value;bool
yT2;Item():value(),yT2(false){}
}
e52
Item>c21;int
x51;ComparisonSet():eL(),c21(),x51(0){}
yO1
AddItem(iK1
a,bool
yT2,n52
type){for(yU3
c=0;c<c21
iT2
c)if(c21[c].value
xB
a)){if(yT2!=c21[c].yT2){iE
cF1
case
iG2:c21.erase(c21.begin()+c);x51
x62
case
iE2:case
iF2:cG1}
tD3
lN1;}
Item
pole;pole.value=a;pole.yT2=yT2;c21
i0
pole)xD1
Ok;}
yO1
AddRelationship(xA
a,xA
b,int
eY1,n52
type){iE
if(eY1==7)cF1
lD
iG2:if(eY1==7){x51
x62}
lD
iE2:case
iF2:if(eY1==0)cG1
break;}
if(!(a.GetHash()<b.GetHash())){a.swap(b);eY1=Swap_Mask(eY1);}
for(yU3
c=0;c<eL
iT2
c){if(eL[c].a
xB
a)&&eL[c].b
xB
b)){iE{int
xI3=x72|eY1;if(xI3==7)cF1
x72=xI3;break
cS2
iE2:case
iF2:{int
xI3=x72&eY1;if(xI3==0)cG1
x72=xI3;break
cS2
iG2:{int
newrel_or=x72|eY1;int
xG2=x72&eY1;iT1
5&&xG2==0){x72=cU3
xD1
lN1;}
iT1
7&&xG2==0){x51+=1;eL.erase(eL.begin()+c)xD1
lN1;}
iT1
7&&xG2==Eq_Mask){x72=Eq_Mask;x51
x62}
y01}
tD3
lN1;}
}
t42
comp;comp.a=a;comp.b=b;comp.relationship=eY1;eL
i0
comp)xD1
Ok;}
}
;n41
yW,typename
CondType>bool
ConstantFolding_LogicCommon(xA&tree,CondType
nY1,bool
xH2){bool
should_regenerate=false;ComparisonSet<yW>comp
lY2
typename
yQ
yO1
xJ3
yQ
Ok;iK1
atree=xL2;switch(atree
nE
iK2
cEqual
lH
Eq_Mask
e62
t81
lH
cU3
e62
cLess
lH
cS3
e62
cLessOrEq
lH
Le_Mask
e62
cGreater
lH
cT3
e62
cGreaterOrEq
lH
Ge_Mask
e62
cNot:xJ3
comp
iU1
l7
0),true
e62
cNotNot:xJ3
comp
iU1
l7
0),false,nY1);break;default:if(xH2||IsLogicalValue(atree))xJ3
comp
iU1,false,nY1);}
switch(change){ReplaceTreeWithZero:tree.ReplaceWithImmed(0)xD1
true;ReplaceTreeWithOne:tree.ReplaceWithImmed(1);nW
yQ
Ok:lD
yQ
BecomeZero
cV
yQ
BecomeOne:tZ
yQ
lN1:yT1
y93
if(should_regenerate){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before ConstantFolding_LogicCommon: "
tU2
#endif
if(xH2){tree.DelParams();}
else{t23{iK1
atree=xL2;if(IsLogicalValue(atree))iL);}
}
e22
comp.c21
iT2
a){if(comp.c21[a].yT2
lA3
cNot);r.cI
r
cV3
nC2!xH2
lA3
cNotNot);r.cI
r
cV3
else
tree.cI}
e22
comp.eL
iT2
a
lA3
cNop);switch(comp.eL[a
tO
iK2
yQ
cS3:r
cC
cLess
cW3
Eq_Mask:r
cC
cEqual
cW3
cT3:r
cC
cGreater
cW3
Le_Mask:r
cC
cLessOrEq
cW3
cU3:r
cC
t81
cW3
Ge_Mask:r
cC
cGreaterOrEq
xC2
r
t11
comp.eL[a].a);r
t11
comp.eL[a].b);r
cV3
if(comp.x51!=0)tree.yG
yW(comp.x51)));
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After ConstantFolding_LogicCommon: "
tU2
#endif
tW1
tD3
e03
x61
ConstantFolding_AndLogic(iF3(tree.GetOpcode()==cAnd
i73()==cAbsAnd)xD1
nK
iE2,true);}
x61
ConstantFolding_OrLogic(iF3(tree.GetOpcode()==cOr
i73()==cAbsOr)xD1
nK
cond_or,true);}
x61
ConstantFolding_AddLogicItems(iF3(tree.GetOpcode()==cAdd)xD1
nK
iG2,false);}
x61
ConstantFolding_MulLogicItems(iF3(tree.GetOpcode()==cMul)xD1
nK
iF2,false);}
}
#include <vector>
#include <map>
#include <algorithm>
l63{using
l63
FUNCTIONPARSERTYPES;using
t4;cN2
CollectionSetBase{enum
x71{Ok,lN1}
;}
nI1
cN2
CollectionSet:public
CollectionSetBase{cN2
yP1{xA
value;xA
yS2;bool
e2;yP1():value(),yS2(),e2(false){}
yP1(iK1
v,iK1
f):value(v),yS2(f),e2(false){}
}
;std::multimap<fphash_t,yP1>i4;typedef
typename
std::multimap<fphash_t,yP1>::xN3
x81;CollectionSet():i4(){}
x81
FindIdenticalValueTo(iK1
value){fphash_t
hash=value.GetHash();for(x81
i=i4.xI2
hash);i!=i4
yQ1
hash;++i){yZ1
xB
i
eA2
value
lN3
i;tD3
i4
tF3;}
bool
Found
n92
x81&b)yZ
b!=i4
tF3;}
x71
AddCollectionTo(iK1
yS2,const
x81&into_which){yP1&c=into_which->second;if(c.e2)c.yS2
cJ
yS2);else{xA
add;add
cC
cAdd);add
t11
c.yS2);add
cJ
yS2);c.yS2.swap(add);c.e2=true;tD3
lN1;}
x71
iV1
iK1
value,iK1
yS2){const
fphash_t
hash=value.GetHash();x81
i=i4.xI2
hash);for(;i!=i4
yQ1
hash;++i){if(i
eA2
value
xB
value
lN3
AddCollectionTo
nL2,i);}
i4.xK3,std::make_pair(hash,yP1(value,yS2)))xD1
Ok;}
x71
iV1
iK1
a)yZ
iV1
a,cH1
1)));}
}
nI1
cN2
ConstantExponentCollection{typedef
t01
TreeSet;typedef
std::eA1
xJ2
e52
xJ2>data;ConstantExponentCollection():data(){}
void
MoveToSet_Unique
n92
yW&eB1&eC1){data
i0
std::eA1(eB1()));data.back().second.swap(eC1
cM3
MoveToSet_NonUnique
n92
yW&eB1&eC1){typename
std::vector<xJ2>::xN3
i=std::xI2
data.iH2
data
tF3,nR3,Compare1st());if(i!=data
yQ1
nW2{i
eA2
xK3
eA2
end(),eC1.iH2
eC1
tF3);}
else{data.xK3,std::eA1(nR3,eC1));}
}
bool
Optimize(){bool
changed=false;std::sort(data.iH2
data
tF3,Compare1st());redo:e22
data
iT2
a){yW
exp_a=data[a].first;iD1
exp_a,yW(1)))y01
for(yU3
b=a+1;b<data
iT2
b){yW
exp_b=data[b].first;yW
xK2=exp_b-exp_a;if(xK2>=fp_abs(exp_a))break;yW
exp_diff_still_probable_integer=xK2*yW(16);if(t52
exp_diff_still_probable_integer)&&!(t52
exp_b)&&!t52
xK2))){TreeSet&a_set=iW1;TreeSet&b_set=data[b].second;
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before ConstantExponentCollection iteration:\n"
;t62
cout);
#endif
if(isEvenInteger(exp_b)&&!isEvenInteger(xK2+exp_a)lG
tmp2;tmp2
x03
tmp2
y92
b_set);tmp2
tZ1
eP1
cAbs);tmp
t11
tmp2);tmp
tZ1
b_set
nY3
1);b_set[0].i22}
a_set.insert(a_set
tF3,b_set.iH2
b_set
tF3);TreeSet
b_copy=b_set;data.erase(data.begin()+b);MoveToSet_NonUnique(xK2,b_copy);nP2
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After ConstantExponentCollection iteration:\n"
;t62
cout);
#endif
goto
redo;}
}
tD3
changed;}
#ifdef DEBUG_SUBSTITUTIONS
void
t62
ostream&out){e22
data
iT2
a){out.precision(12);out<<data[a].first<<": "
;iM1
iW1
iT2
b){if(b>0)out<<'*';lX3
iW1[b],out);}
out<<l53}
}
#endif
}
nI1
static
xA
nS1
xA&value,bool&xF){switch(value
nE
iK2
cPow:{xA
eB2
value
l7
1);value.x91
return
nR3
cS2
cRSqrt:value.x91
xF=true
xD1
cH1-0.5));case
cInv:value.x91
xF=true
xD1
cH1-1));default:break;tD3
cH1
1));}
yY1
void
eD1
e61&mul,x53,iK1
yS2,bool&yS1
bool&xF){e22
xW++a
lG
value
iH
l7
a));xA
nR3(nS1
value,xF));if(!yS2
xF1||yS2
nJ1!=yW(1.0)lG
cI1;cI1
x03
cI1
cJ
nW2;cI1
cJ
yS2);cI1
tZ1
nR3.swap(cI1);}
#if 0 /* FIXME: This does not work */
yZ1
nE==cMul){if(1){bool
exponent_is_even=nR3
xF1&&isEvenInteger(nR3
nJ1);iM1
value.cX3{bool
tmp=false;xA
val(value
l7
b));xA
exp(nS1
val,tmp));if(exponent_is_even||(exp
xF1&&isEvenInteger(exp
nJ1))lG
cI1;cI1
x03
cI1
cJ
nW2;cI1
t11
exp);cI1.ConstantFolding();if(!cI1
xF1||!isEvenInteger(cI1
nJ1)){goto
cannot_adopt_mul;}
}
}
}
eD1
mul,value,nR3,yS1
xF);}
else
cannot_adopt_mul:
#endif
{if(mul.iV1
value,nW2==CollectionSetBase::lN1)yT1}
}
}
x61
ConstantFolding_MulGrouping(cZ2
bool
xF=false;bool
should_regenerate=false;e61
mul;eD1
mul,tree,cH1
1)),yS1
xF);typedef
std::pair<xA,t01>eE1;typedef
std::multimap<fphash_t,eE1>c31;c31
t7;e72
e61::x81
j=mul.i4.xM3
j!=mul.i4
tF3;++j
lG&value=j
eA2
value;xA&eB2
j
eA2
yS2;if(j
eA2
e2)nR3
tZ1
const
fphash_t
eF1=nR3.GetHash();typename
c31::xN3
i=t7.xI2
eF1);for(;i!=t7
yQ1
eF1;++i)if(i
eA2
first
xB
nW2){if(!nR3
xF1||!cJ1
nJ1,yW(1)))yT1
i
eA2
second
i0
value);goto
skip_b;}
t7.xK3,std::make_pair(eF1,std::make_pair(nR3,t01(yU3(1),value))));skip_b:;}
#ifdef FP_MUL_COMBINE_EXPONENTS
ConstantExponentCollection<yW>cK1;e72
c31::xN3
j,i=t7.xM3
i!=t7
tF3;i=j){j=i;++j;eE1&list=i->second;if
tG3
xS3
eB2
list.first
nJ1;if(!(nR3==yW(0)))cK1.MoveToSet_Unique(nR3,list.second);t7.erase(i);}
}
if(cK1.Optimize())yT1
#endif
if(should_regenerate
lG
before=tree;before.iT
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before ConstantFolding_MulGrouping: "
;lX3
before)xL1"\n"
;
#endif
tree.DelParams();e72
c31::xN3
i=t7.xM3
i!=t7
tF3;++i){eE1&list=i->second;
#ifndef FP_MUL_COMBINE_EXPONENTS
if
tG3
xS3
eB2
list.first
nJ1;if(nR3==yW(0))y01
if(cJ1
nI3
tree.AddParamsMove(list.second);y01}
}
#endif
xA
mul;mul
x03
mul
y92
list.second);mul
tZ1
if(xF&&list.first
xF1){if
tG3
nJ1==yW(1)/yW(3)lG
cbrt;cbrt
cC
cCbrt);cbrt.e7
cbrt
yP
cbrt);y01
t5
0.5)lG
sqrt;sqrt
cC
cSqrt);sqrt.e7
sqrt
yP
sqrt);y01
t5-0.5)lG
rsqrt;rsqrt
cC
cRSqrt);rsqrt.e7
rsqrt
yP
rsqrt);y01
t5-1)lG
inv;inv
cC
cInv);inv.e7
inv
yP
inv);y01}
}
xA
pow;pow
cC
cPow);pow.e7
pow
t11
list.first);pow
yP
pow);}
#ifdef FP_MUL_COMBINE_EXPONENTS
t7.clear()xG1
tU
iT2
a){yW
eB2
tU[a].first;if(cJ1
nI3
tree.AddParamsMove(tU[a].second);y01}
xA
mul;mul
x03
mul
y92
tU[a].second);mul
tZ1
xA
pow;pow
cC
cPow);pow.e7
pow.yG
nW2);pow
yP
pow);}
#endif
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After ConstantFolding_MulGrouping: "
tU2
#endif
return!tree
xB
before);tD3
e03
x61
ConstantFolding_AddGrouping(cZ2
bool
should_regenerate=false;e61
add
lY2
c92
nE==cMul)y01
if(add.AddCollection
iH
l7
a))==CollectionSetBase::lN1)yT1}
std::vector<bool>remaining
iH
cB1);yU3
t8=0
lY2
iK1
mulgroup=xL2;if(mulgroup
nE==cMul){iM1
t21
cX3{if(mulgroup
l7
b)xF1)y01
typename
e61::x81
c=add.FindIdenticalValueTo(mulgroup
l7
b));if(add.Found(c)lG
tmp(mulgroup
xH1
CloneTag());tmp.iY1
b);tmp
tZ1
add.AddCollectionTo(tmp,c);yT1
goto
done_a;}
}
remaining[a]=true;t8+=1;done_a:;}
}
if(t8>0){if(t8>1){std::vector<std::pair<xA,yU3> >nV;std::multimap<fphash_t,yU3>eG1;bool
lB3=false
xG1
xW
iN1{iM1
xL2.cX3{iK1
p=xL2
l7
b);const
fphash_t
p_hash=p.GetHash();for(std::multimap<fphash_t,yU3>::const_iterator
i=eG1.xI2
p_hash);i!=eG1
yQ1
p_hash;++i){if(nV[i->second].first
xB
p)){nV[i->second].second+=1;lB3=true;goto
found_mulgroup_item_dup;}
}
nV
i0
std::make_pair(p,yU3(1)));eG1.insert(std::make_pair(p_hash,nV.yD3));found_mulgroup_item_dup:;}
}
if(lB3
lG
eC2;{yU3
max=0;for(yU3
p=0;p<nV
iT2
p)if(nV[p].second<=1)nV[p].second=0;else{nV[p].second*=nV[p].first
xS2;if(nV[p].second>max){eC2=nV[p].first;max=nV[p].second;}
}
}
xA
group_add;group_add
cC
cAdd);
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Duplicate across some trees: "
;lX3
eC2)xL1" in "
tU2
#endif
e22
xW
iN1
iM1
xL2.cX3
if(eC2
xB
xL2
l7
b))lG
tmp
iH
l7
a)xH1
CloneTag());tmp.iY1
b);tmp
tZ1
group_add
t11
tmp);remaining[a]=false;break;}
group_add
tZ1
xA
group;group
x03
group
t11
eC2);group
t11
group_add);group
tZ1
add.iV1
group);yT1}
}
e22
xW
iN1{if(add.AddCollection
iH
l7
a))==CollectionSetBase::lN1)yT1}
}
if(should_regenerate){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before ConstantFolding_AddGrouping: "
tU2
#endif
tree.DelParams();e72
e61::x81
j=add.i4.xM3
j!=add.i4
tF3;++j
lG&value=j
eA2
value;xA&coeff=j
eA2
yS2;if(j
eA2
e2)coeff
tZ1
if(coeff
xF1){iD1
coeff
nJ1,yW(0)))y01
iD1
coeff
nJ1
nI3
tree
t11
value);y01}
}
xA
mul;mul
x03
mul
t11
value);mul
t11
coeff);mul
yP
mul);}
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After ConstantFolding_AddGrouping: "
tU2
#endif
tW1
tD3
e03}
l63{using
l63
FUNCTIONPARSERTYPES;using
t4
nI1
bool
ConstantFolding_IfOperations(iF3(tree.GetOpcode()==cIf
i73()==cAbsIf);for(;;){yU2
nE==cNot){n23
cIf);i42
t31
0)yZ2
l7
1).swap
iH
l7
2));}
else
yU2
cA1{n23
cY3;i42
t31
0)yZ2
l7
1).swap
iH
l7
2));}
else
xX2
tB
0),eP2
cY3){lD2
tree
t31
1));nW
iY2
tree
t31
2));nW
lH1
yU2
nE==cIf||i42
nE==cAbsIf
lG
cond=i42;xA
lC3;lC3
n33==cIf?cNotNot:cAbsNotNot);lC3
xM2
1));ConstantFolding(lC3);xA
lD3;lD3
n33==cIf?cNotNot:cAbsNotNot);lD3
xM2
2));ConstantFolding(lD3);if(lC3
xF1||lD3
xF1
lG
eQ;eQ
n33);eQ
xM2
1));eQ.nL
1));eQ.nL
2));eQ
tZ1
xA
eR;eR
n33);eR
xM2
2));eR.nL
1));eR.nL
2));eR
tZ1
n23
cond
nE);i21
0,cond
yZ2.lO1
1,eQ);tree.lO1
2,eR)xA1}
if
iH
l7
1)nE==tree
l7
2)nE&&iH
l7
1)nE==cIf||e02
nE==cY3
lG&cZ3=e02;xA&leaf2=tree
l7
2);if
eH2
0)x01
0))&&eH2
1)x01
1))||cZ3
l7
2)x01
2)))lG
eQ;eQ
iP1
eQ.nL
0));eQ
xN2
1));eQ
xO2
1));eQ
tZ1
xA
eR;eR
iP1
eR.nL
0));eR
xN2
2));eR
xO2
2));eR
tH
SetParam(0,cZ3
yZ2.lO1
1,eQ);tree.lO1
2,eR)xA1
if
eH2
1)x01
1))&&cZ3
l7
2)x01
2))lG
eS;eS
iP1
eS.AddParamMove
iH
l7
0));eS
xN2
0));eS
xO2
0));eS
tH
lO1
0,eS);i21
2,cZ3
l7
2
lE3
cZ3
l7
1))xA1
if
eH2
1)x01
2))&&cZ3
l7
2)x01
1))lG
eD2;eD2
cC
leaf2
nE==cIf?cNot:yL3);eD2
xO2
0));eD2
tZ1
xA
eS;eS
iP1
eS.AddParamMove
iH
l7
0));eS
xN2
0));eS
t11
eD2);eS
tH
lO1
0,eS);i21
2,cZ3
l7
2
lE3
cZ3
l7
1))xA1}
xA&xY=e02;xA&y1=tree
l7
2);if(xY
xB
y1)){tree
t31
1))xA1
const
OPCODE
op1=xY
nE;const
OPCODE
op2=y1
nE;if
c03
op2){if(xY
yU1
1
lG
lN
0
nB2
0))n62
lU
if(xY
yU1
2&&y1
yU1
2){if(xY
l7
0)xB
y1
l7
0))lG
param0=xY
l7
0);xA
lN
1
nB2
1))n62
n61
param0
lU
if(xY
l7
1)xB
y1
l7
1))lG
param1=xY
l7
1);xA
lN
0
nB2
0))n62
n61
lM1
n61
param1)xA1}
if
c03
xO3
cMul
iX1
cAnd
iX1
cOr
iX1
cAbsAnd
iX1
cAbsOr
iX1
cMin
iX1
cMax){t01
lF3;c8{for(yU3
b=y1.l01
b-->0;){if(xY
l7
a)xB
y1
l7
b))){if(lF3
i23){xY.iT
y1.iT}
lF3
i0
xY
l7
a));y1.iY1
b);xY.iY1
a
xC2}
}
if(!lF3
i23){xY
tZ1
y1
tZ1
xA
lM1;lM1
iP1
lM1.SetParamsMove
iH.iG1))y8
SetParamsMove(lF3
lU}
}
if
c03
xO3
cMul||c03
cAnd
nA1
y1))||c03
cOr
nA1
y1))){c8
if(xY
l7
a)xB
y1)){xY.iT
xY
i33
xY
tZ1
xA
c41=y1;y1=lE1
op1==xO3
cOr)l8
op1
n61
c41
lU}
if(c03
cAnd
iX1
cOr)&&op2==cNotNot
lG&lG3=y1
l7
0);c8
if(xY
l7
a)xB
lG3)){xY.iT
xY
i33
xY
tZ1
xA
c41=lG3;y1=lE1
op1
xP3
op1
n61
c41
lU}
if(op2==cAdd||op2==cMul||(op2==cAnd
nA1
xY))||(op2==cOr
nA1
xY))){for
e51
y1
yV1
y1
l7
a)xB
xY)){y1.iT
y1
i33
y1
tZ1
xA
c51=xY;xY=lE1
op2==cAdd||op2
xP3
op2
n61
c51
lU}
if((op2==cAnd||op2==cOr)&&op1==cNotNot
lG&lH3=xY
l7
0)xR3
y1
yV1
y1
l7
a)xB
lH3)){y1.iT
y1
i33
y1
tZ1
xA
c51=lH3;xY=lE1
op2
xP3
op2
n61
c51
lU
tD3
e03}
#include <limits>
l63{using
l63
FUNCTIONPARSERTYPES;using
t4
nI1
int
maxFPExponent()yZ
std::numeric_limits<yW>::max_exponent;}
x61
nT1
yW
e13
nW2{if(base
t53
tW1
iD1
e13(0))||lI3
e13(1))y5
return
nR3>=yW(maxFPExponent
t63/fp_log2(base);}
x61
ConstantFolding_PowOperations(iF3(tree.GetOpcode()==cPow);iO3
e02
xS3
const_value
xV3
n0,xK);tree.ReplaceWithImmed(const_value)xD1
e03
if
iH
tS2
lI3
xK
nI3
tree
t31
0))xA1
iO3
lI3
n0,yW(1)nJ
1)xD1
e03
iO3
e02
nE==cMul){bool
xP2=false;yW
iZ1=n0;xA
mulgroup=e02
xR3
mulgroup
yV1
mulgroup
l7
a)xS3
imm=mulgroup
l7
a)nJ1;{if(nT1
iZ1,imm))break;yW
l02
xV3
iZ1,imm);iD1
l02,yW(0)))break;if(!xP2){xP2=true;t21
iT}
iZ1=l02;t21
iY1
a
xC2}
if(xP2){t21
Rehash();
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before pow-mul change: "
tU2
#endif
i42.Become(yW1
iZ1));e02.Become(mulgroup);
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After pow-mul change: "
tU2
#endif
}
}
if
iH
tS2
i42
nE==cMul){yW
l12=xK;yW
xQ2=1.0;bool
xP2=false;xA&mulgroup=i42
xR3
mulgroup
yV1
mulgroup
l7
a)xS3
imm=mulgroup
l7
a)nJ1;{if(nT1
imm,l12))break;yW
eH1
xV3
imm,l12);iD1
eH1,yW(0)))break;if(!xP2){xP2=true;t21
iT}
xQ2*=eH1;t21
iY1
a
xC2}
if(xP2){t21
Rehash();xA
c13;c13
cC
cPow);c13.SetParamsMove
iH.iG1));c13.y82);n23
cMul
n61
c13);tree
cJ
yW1
xQ2))xA1}
yU2
nE==cPow&&e02
xF1&&i42
l7
1)xS3
a=i42
l7
1)nJ1;yW
b=xK;yW
c=a*b;if(isEvenInteger(a)&&!isEvenInteger(c)lG
lJ3;lJ3
cC
cAbs);lJ3.nL
0)l7
0));lJ3
tZ1
tree.lO1
0,lJ3);}
else
i21
0,i42
l7
0
lE3
yW1
c));tD3
e03}
l63{using
l63
FUNCTIONPARSERTYPES;using
t4;cN2
l5{enum
eE2{nU2=0,c61=1,t72=2,lQ3=3,MakeNotNotP0=4,MakeNotNotP1=5,MakeNotP0=6,MakeNotP1=7,xE=8
cO3
l22{Never=0,Eq0=1,Eq1=2,xW3=3,xX3=4}
;eE2
if_identical;eE2
if_always[4];cN2{eE2
what:4;l22
when:4;}
tK1,tL1,tM1,tN1
nI1
eE2
Analyze(iK1
a,iK1
b
tM3{if(a
xB
b
lN3
if_identical;lK3
tM
a);lK3
p1=nQ1(b);if(p0
yS&&p1.min.known){if(p0
xN<p1
i61
tO1
0]tV
0];if(p0
xN<=p1
i61
tO1
1]tV
1];}
if(lC2
p1
yS){if(p0
i61>p1
xN
tO1
2]tV
2];if(p0
i61>=p1
xN
tO1
3]tV
3];}
if(IsLogicalValue(a)){if(tK1
xQ3
tK1.when,p1
lN3
tK1.what;if(tM1
xQ3
tM1.when,p1
lN3
tM1.what;}
if(IsLogicalValue(b)){if(tL1
xQ3
tL1.when,p0
lN3
tL1.what;if(tN1
xQ3
tN1.when,p0
lN3
tN1.what;tD3
xE;}
yY1
bool
TestCase(l22
when,const
lK3&p){if(!p.min.known||!p
yS
y5
switch(when
iK2
Eq0
nC1==yW(0.0)&&p
xN==p
i61;case
Eq1
nC1==yW(1.0)&&p
xN==p
xN;case
xW3
nC1>i13
p
xN<=yW(1);case
xX3
nC1
yJ2&&p
xN<yW(1);default:;tD3
e03}
;l63
RangeComparisonsData{static
const
l5
Data[6]={{l5::c61
cP
tP
xR2
tP
xE
nD1
Eq1
nE1
Eq1
nF1
Eq0
nG1
Eq0}
}
cP
nU2
cP
c61,l5::xR2
c61,l5::xE
nD1
Eq0
nE1
Eq0
nF1
Eq1
nG1
Eq1}
}
cP
nU2
cP
c61,l5::t72,l5::tP
nU2
nF1
xW3
nE1
xX3
xT2
c61
cP
xR2
c61,l5::tP
lQ3
nF1
xX3
nE1
xW3
xT2
nU2
cP
tP
tP
c61,l5::t72
nD1
xX3
nG1
xW3
xT2
c61
cP
tP
lQ3,l5::xR2
c61
nD1
xW3
nG1
xX3}
cP
xR2
t9}
;}
x61
ConstantFolding_Comparison(cZ2
using
l63
RangeComparisonsData;assert(tree.GetOpcode()>=cEqual&&tree.GetOpcode()<=cGreaterOrEq);switch(Data[tree
nE-cEqual].Analyze
iH
l7
0),e02)iK2
l5::nU2:tree.ReplaceWithImmed(0)xU2
c61:tree.ReplaceWithImmed(1)xU2
lQ3:n23
cEqual)xU2
t72:n23
t81)xU2
MakeNotNotP0:n23
cNotNot)eZ1
1)xU2
MakeNotNotP1:n23
cNotNot)eZ1
0)xU2
MakeNotP0:n23
cNot)eZ1
1)xU2
MakeNotP1:n23
cNot)eZ1
0)xU2
xE:;}
if
iH
l7
1)xF1)switch
iH
l7
0)nE
iK2
cAsin:lL
fp_sin(iI2
cAcos:lL
fp_cos(xK)));n23
eP2
cLess?cGreater:eP2
cLessOrEq?cGreaterOrEq:eP2
cGreater?cLess:eP2
cGreaterOrEq?cLessOrEq:tree
nE);nW
cAtan:lL
fp_tan(iI2
cLog:lL
fp_exp(iI2
cSinh:lL
fp_asinh(iI2
cTanh:if(fp_less(fp_abs(xK)nI3
lL
fp_atanh(xK)))xA1
break;default:break;tD3
e03}
#include <list>
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63{
#ifdef DEBUG_SUBSTITUTIONS
yH
double
d){union{double
d;uint_least64_t
h;}
t82
d=d;lA1
h
nH1
#ifdef FP_SUPPORT_FLOAT_TYPE
yH
float
f){union{float
f;uint_least32_t
h;}
t82
f=f;lA1
h
nH1
#endif
#ifdef FP_SUPPORT_LONG_DOUBLE_TYPE
yH
long
double
ld){union{long
double
ld;cN2{uint_least64_t
a;unsigned
short
b;}
s;}
t82
ld=ld;lA1
s.b<<data.s.a
nH1
#endif
#ifdef FP_SUPPORT_LONG_INT_TYPE
yH
long
ld){o<<"("
<<std::hex<<ld
nH1
#endif
#endif
}
t4{lM
nF)){}
lM
const
yW&i
xH1
nV3
nF
i)){data
x9
#ifdef __GXX_EXPERIMENTAL_CXX0X__
lM
yW&&i
xH1
nV3
nF
std::move(i))){data
x9
#endif
lM
unsigned
v
xH1
VarTag
nF
iN2,v)){data
x9
lM
cR2
o
xH1
OpcodeTag
nF
o)){data
x9
lM
cR2
o
cQ2
f
xH1
FuncOpcodeTag
nF
o,f)){data
x9
lM
iK1
b
xH1
CloneTag
nF*b.data)){}
eQ1
xA::~CodeTree(){}
lB
ReplaceWithImmed
n92
yW&i){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Replacing "
;lX3*this);if(IsImmed())OutFloatHex(l13,GetImmed())xL1" with const value "
<<i;OutFloatHex(l13,i)xL1"\n"
;
#endif
data=new
x12<yW>(i);}
eQ1
cN2
ParamComparer{iC2()(iK1
a,iK1
b
tM3{if(a
xS2!=b
xS2)return
a
xS2<b
xS2
xD1
a.GetHash()<b.GetHash();}
}
x43
x12<yW>::Sort(){switch(Opcode
iK2
cAdd:case
cMul:case
cMin:case
cMax:case
cAnd:case
cAbsAnd:case
cOr:case
cAbsOr:case
cHypot:case
cEqual:case
t81:std::sort(iU2
iH2
iU2
end(),ParamComparer
t63;lD
cLess
lZ
cGreater;}
lD
cLessOrEq
lZ
cGreaterOrEq;}
lD
cGreater
lZ
cLess;}
lD
cGreaterOrEq
lZ
cLessOrEq;}
break;default:y93
lB
AddParam(iK1
param){xZ
i0
param);}
lB
n12
xA&param){xZ
i0
xA());xZ.back().swap
iS3);}
lB
SetParam(yU3
which,iK1
b){nU1
which
iJ2
xZ[which]=b;}
lB
lO1
yU3
which,xA&b){nU1
which
iJ2
xZ[which
t03
b);}
lB
AddParams
n92
nM){xZ.insert(xZ
tF3,l32.iH2
l32
tF3);}
lB
AddParamsMove(nM){yU3
endpos=xZ.size(),added=l32.size();xZ
nY3
endpos+added,xA());for(yU3
p=0;p<added;++p)xZ[endpos+p
t03
l32[p]);}
lB
AddParamsMove(nM,yU3
l42){nU1
l42
iJ2
iY1
l42);AddParamsMove(eS1}
lB
SetParams
n92
nM){t01
tmp(eS1
xZ.i22}
lB
SetParamsMove(nM){xZ.swap(eS1
l32.clear();}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
lB
SetParams(t01&&l32){SetParamsMove(eS1}
#endif
lB
iY1
yU3
index){t01&yV3=xZ;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
iU2
erase(iU2
begin()+index);
#else
yV3[index].data=0;for(yU3
p=index;p+1<yV3
iT2
p)yV3[p].data.UnsafeSetP(&*yV3[p+1
iJ2
yV3[x13-1].data.UnsafeSetP(0);iU2
resize(x13-1);
#endif
}
lB
DelParams(){xZ.clear();}
x61
xA::IsIdenticalTo(iK1
b
tM3{if(&*data==&*b.data)return
true
xD1
data->IsIdenticalTo(*b.data);}
x61
x12<yW>::IsIdenticalTo
n92
x12<yW>&b
tM3{if(Hash!=b.Hash
y5
if(Opcode!=tN3
y5
switch(Opcode
iK2
cImmed:return
lI3
Value,tO3;case
iN2:return
iI1==b.iH1
case
cFCall:case
cPCall:if(iI1!=b.iI1
y5
break;default:break;}
if(x13!=b.x13
y5
e22
yV3
iT2
a){if(!yV3[a]xB
b.yV3[a])y5}
tW1}
lB
Become(iK1
b){if(&b!=this&&&*data!=&*b.data){DataP
tmp=b.data;iT
data.i22}
}
lB
CopyOnWrite(){if(GetRefCount()>1)data=new
x12<yW>(*data);}
eQ1
xA
xA::GetUniqueRef(){if(GetRefCount()>1)return
xA(*this,CloneTag())xD1*this;}
tQ):c0
cNop
eW3(),nA
tQ
const
x12&b):c0
tN3
eW3(tO3,iI1(b.iI1),yV3(b.yV3),Hash(b.Hash),Depth(b.Depth),t41
b.iJ1){}
tQ
const
yW&i):c0
cImmed
eW3(i),nA
#ifdef __GXX_EXPERIMENTAL_CXX0X__
tQ
x12<yW>&&b):c0
tN3
eW3
c12
tO3),iI1(b.iI1),yV3
c12
b.yV3)),Hash(b.Hash),Depth(b.Depth),t41
b.iJ1){}
tQ
yW&&i):c0
cImmed
eW3
c12
i)),nA
#endif
tQ
cR2
o):c0
o
eW3(),nA
tQ
cR2
o
cQ2
f):c0
o
eW3(),iI1(f),yV3(),Hash(),Depth(1),t41
0){}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <iostream>
using
l63
FUNCTIONPARSERTYPES;
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
l63{e01
l52(nQ,std
cK&done,std::ostream&o){e22
xW++a)l52
iP3
done,o);std::ostringstream
buf;DumpTree
iH,buf);done[tree.GetHash()].insert(buf.str());}
}
#endif
t4{
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
e01
DumpHashes(tY){std
cK
done;l52
iH,done,o);for(std
cK::const_iterator
i=done.xM3
i!=done
tF3;++i){const
std::set<std::string>&flist=i->second;if(flist.size()!=1)o<<"ERROR - HASH COLLISION?\n"
;for(std::set<std::string>::const_iterator
j=flist.xM3
j!=flist
tF3;++j){o<<'['<<std::hex<<i->first.hash1<<','<<i->first.hash2<<']'<<std::dec;o<<": "
<<*j<<"\n"
;}
}
}
e01
lX3
tY){l33
iD3;switch
iH
nE
iK2
cImmed:o<<tree
nJ1
xY3
iN2:o<<"Var"
<<iH.GetVar()-iN2)xY3
cAdd:iD3"+"
;lD
cMul:iD3"*"
;lD
cAnd:iD3"&"
;lD
cOr:iD3"|"
;lD
cPow:iD3"^"
;break;default:iD3;o<<FP_GetOpcodeName
iH
nE);tW2
cFCall||eP2
cPCall)o<<':'<<tree.GetFuncNo();}
o<<'(';if
iH
cB1<=1&&sep2[1])o<<(sep2+1)<<' 'lY2
if(a>0)o<<' ';DumpTree
iP3
o);if(a+1<tree
cB1)o<<sep2;}
o<<')';}
e01
DumpTreeWithIndent(tY,const
std::string&indent){o<<'['<<std::hex<<(void*)(&tree.iG1))<<std::dec<<','<<tree.GetRefCount()<<']';o<<indent<<'_';switch
iH
nE
iK2
cImmed:o<<"cImmed "
<<tree
nJ1;o<<'\n'xY3
iN2:o<<"VarBegin "
<<iH.GetVar()-iN2);o<<'\n'xD1;default:o<<FP_GetOpcodeName
iH
nE);tW2
cFCall||eP2
cPCall)o<<':'<<tree.GetFuncNo();o<<'\n';}
e22
xW++a){std::string
ind=indent;for(yU3
p=0;p<ind.size();p+=2)if(ind[p]=='\\')ind[p]=' ';ind+=(a+1<tree
cB1)?" |"
:" \\"
;DumpTreeWithIndent
iP3
o,ind);}
o<<std::flush;}
#endif
}
#endif
using
l63
l21;using
l63
FUNCTIONPARSERTYPES;
#include <cctype>
l63
l21{x61
ParamSpec_Compare
n92
void*aa,const
void*bb,n22
type){switch(type
iK2
n72
xG&a=*(xG*)aa;xG&b=*(xG*)bb
xD1
a
tP1==b
tP1&&a.index==b.index&&a.depcode==b.depcode
cS2
NumConstant:{eF&a=*(eF*)aa;eF&b=*(eF*)bb
xD1
lI3
a
cC2,b
cC2)&&a.modulo==b.modulo
xK1
xH&a=*(xH*)aa;xH&b=*(xH*)bb
xD1
a
tP1==b
tP1&&a
xE1==b
xE1&&a.data.match_type==b.data.match_type&&a.data
y52==b.data
y52&&a.data.param_list==b.data.param_list&&a
xU3==b
xU3&&a.depcode==b.depcode;}
}
tW1}
unsigned
ParamSpec_GetDepCode
n92
cW2&b){switch(b.first
iK2
n72
y03*s=n92
xG*)b.second
xD1
s->depcode
xK1
const
xH*s=n92
xH*)b.second
xD1
s->depcode;}
default:break;tD3
0;}
e01
DumpParam
n92
cW2&y62
std::ostream&o){static
const
char
ParamHolderNames[][2]={"%"
,"&"
,"x"
,"y"
,"z"
,"a"
,"b"
,"c"
}
;unsigned
xV2
0;xT3
NumConstant:{const
eF&n82
eF*xJ1;o.precision(12);o<<param
cC2;break
cS2
n72
y03&n82
xG*xJ1;o<<ParamHolderNames[param.index];xV2
param
tP1;break
xK1
const
xH&n82
xH*xJ1;xV2
param
tP1;yI
GroupFunction){xZ3
xE1==cNeg){o<<"-"
;n2}
nC2
param
xE1==cInv){o<<"/"
;n2}
else{std::string
opcode=FP_GetOpcodeName((cR2)param
xE1).substr(1)xG1
opcode
iT2
a)opcode[a]=(char)std::toupper(opcode[a]);o<<opcode<<"( "
;n2
o<<" )"
;}
}
else{o<<'('<<FP_GetOpcodeName((cR2)param
xE1)<<' ';yI
PositionalParams)o<<'[';yI
SelectedParams)o<<'{';n2
xZ3
xU3!=0)o<<" <"
<<param
xU3<<'>';yI
PositionalParams)o<<"]"
;yI
SelectedParams)o<<"}"
;o<<')';}
y93
switch(ImmedConstraint_Value(xW2
ValueMask)iK2
ValueMask:lD
Value_AnyNum:lD
nG2:o<<"@E"
;lD
Value_OddInt:o<<"@O"
;lD
t51:o<<"@I"
;lD
Value_NonInteger:o<<"@F"
;lD
nA2:o<<"@L"
;xX2
ImmedConstraint_Sign(xW2
SignMask)iK2
SignMask:lD
Sign_AnySign:lD
lP1:o<<"@P"
;lD
eI1:o<<"@N"
;xX2
ImmedConstraint_Oneness(xW2
OnenessMask)iK2
OnenessMask:lD
Oneness_Any:lD
Oneness_One:o<<"@1"
;lD
Oneness_NotOne:o<<"@M"
;xX2
ImmedConstraint_Constness(xW2
ConstnessMask)iK2
ConstnessMask:lD
nZ2:if(lT3.first==ParamHolder){y03&n82
xG*xJ1;xZ3.index<2)break;}
o<<"@C"
;lD
Constness_NotConst:o<<"@V"
;lD
Oneness_Any:y93
e01
DumpParams
iK
paramlist
cQ2
count,std::ostream&o){for
iK
a=0;a<count;++a){if(a>0)o<<' ';const
cW2&param=lU1
paramlist,a);DumpParam<yW>iS3,o);unsigned
depcode=ParamSpec_GetDepCode
iS3);if(depcode!=0)o<<"@D"
<<depcode;}
}
}
#include <algorithm>
using
l63
l21;using
l63
FUNCTIONPARSERTYPES;l63{y03
plist_p[37]={{2,0,x02
2,0,0x4}
,{2,lP1,x02
2,eI1|Constness_NotConst,x02
2,Sign_NoIdea,x02
2,nA2,x02
3,Sign_NoIdea,x02
3,0,x02
3,nA2,x02
3,0,0x8}
,{3,Value_OddInt,x02
3,Value_NonInteger,x02
3,nG2,x02
3,lP1,x02
0,eI1|lV{0,lV{0,lP1|lV{0,nG2|lV{0,nZ2,0x1}
,{0,t51|lP1|lV{0,t61
nZ2,0x1}
,{0,t61
lV{0,Oneness_One|lV{0,nA2|lV{1,lV{1,nG2|lV{1,t61
lV{1,t51|lV{1,lP1|lV{1,eI1|lV{6,0,x02
4,0,x02
4,t51,x02
4,lV{4,0,0x16}
,{5,0,x02
5,lV}
nI1
cN2
plist_n_container{static
const
eF
plist_n[20];}
nI1
const
eF
plist_n_container<yW>::plist_n[20]={{yW(-2
eU1-1
eU1-0.5
eU1-0.25
eU1
0),0}
,{fp_const_deg_to_rad<x92
fp_const_einv<x92
fp_const_log10inv<e23
0.5),0}
,{fp_const_log2<e23
1),0}
,{fp_const_log2inv<e23
2),0}
,{fp_const_log10<x92
fp_const_e<x92
fp_const_rad_to_deg<x92-fp_const_pihalf<e33{yW(0),xO1{fp_const_pihalf<e33{fp_const_pi<e33}
;const
xH
plist_s[511]={{{1,15,t92
397,t92
471,t92
15,cNeg,GroupFunction,0}
,nZ2,0x1}
,{{1,15,xY2
24,xY2
459,xY2
460,xY2
492,cInv
yG3
333122
y3
lR3
48276
y3
l6
260151
y3
l6
464028
y3
l6
172201
y3
l6
48418
y3
l6
1331
y3
l6
172354
y3
l6
39202
y3
l6
312610
y3
l6
470469
y3
l6
296998
y3
l6
47
y3
SelectedParams,0
e63
y7
162863
y3
l6
25030
y3
l6
7168
y3
l6
199680
y3
l6
35847
y3
l6
60440
y3
l6
30751
y3
l6
186549
y3
l6
270599
y3
l6
60431
y3
l6
259119
y3
l6
291840
y3
l6
283648
y3
l6
375027
y3
l6
235748
y3
l6
235754
y3
l6
18676
y3
l6
61684
y3
l6
241938
y3
l6
242974
y3
l6
31751
y3
l6
372758
y3
SelectedParams,0
e63
y7
373999
y3
l6
283969
y3
l6
283971
y3
l6
283976
y3
l6
354351
y3
l6
375112
y3
l6
245767
y3
l6
354342
y3
l6
355366
y3
SelectedParams,0}
,0,0x5
y7
355366
y3
l6
250927
y3
SelectedParams,0}
,0,0x1
y7
321583
y3
l6
297007
y3
l6
15814
y3
l6
355375
y3
l6
355375
y3
SelectedParams,0}
,0,0x5
y7
162871
y3
l6
258103
y3
i5
0,0
y3
nH
0,0
t71
1,45
y3
nH
1,53
y3
nH
1,54
y3
nH
1,55
y3
nH
1,56
y3
nH
1,26
y3
nH
1,259
eF2
0x16}
,{{1,253
y3
nH
1,272
t71
1,327
eF2
0x16}
,{{1,437
eF2
e83
439
eF2
e83
0
eF2
e83
0
i1
2
e63}
,{{1,0
y3
nH
1,21
y3
nH
1,15
y3
nH
1,24
i1
2}
,0,0x0
y7
58392
t71
0,0
i1
1}
,lP1,0x0
y7
24591
e73
33807
e73
48143
e73
289816
e73
295960
e73
307200,l9
315470,l9
39202,l9
121894,l9
421926,l9
429094,l9
437286,l9
289073,l9
325002,l9
334218,l9
7627,l9
7700,l9
7724,l9
38,l9
50587,l9
405504,l9
31751,l9
404487,l9
76816,lQ1
318479,l9
319503,lR1
7340060,lQ1
332833,l9
329764,lR1
7340176,l9
89387,l9
339273,l9
332170,l9
487462,l9
492582,l9
497702,l9
509990,l9
336947,l9
342016,l9
518182,l9
503808,l9
286727,l9
91151,l9
131087,l9
328719,lQ1
7217,l9
304143,l1
0x14
y7
296976
iV2
7340056
iV2
7340061,lQ1
7192,l9
7447,l9
24583,l9
335887,l1
0x10
y7
7206,l9
7168,l9
343078,l1
0x6
y7
350246,l9
352294,l9
426398,l1
0x12}
,{{3,40272286,lQ1
484766,l1
0x12}
,{{3,40330654,lQ1
50600,l9
426456,lQ1
484824,lQ1
296998,l1
e93
428070,lR1
40330712,lQ1
50665,l9
357414,l1
0x6
y7
38372,lR1
47655936,l9
471040,lR1
64009216,l9
562176,l9
357391,l1
0x7
y7
343097,l1
0x7
y7
39265,l9
39268,l1
e93
39338
iV2
15779300,l9
343055,l1
0x7}
,{{3,15779238,l9
436262,l9
502822,l9
39265,l1
e93
58768,l9
39403,l1
e93
121856,l9
35847,l9
15,l1
e93
366630,l9
15760,l9
15,l9
122904,l9
121880,l9
30751,l9
15681,lR1
67573791,l9
57,lR1
7340177,l9
39240,l9
354319,l1
0x1
y7
62918,l1
x02{0,0,nY
0,0,iC
2,cL1
2,cM1
3,cL1
3,cM1
38,nY
1,38,iC
14,nY
1,57,nY
1,16,nY
2,464959,lJ
1
iW2
306,nY
1,327,lJ
2}
,0,0x0
y7
465223,lJ
1}
,0,0x16}
,{{1,292,cL1
293,cM1
294,nY
1,295,iC
400,nY
1,0,nY
1,454,nY
1,459,nY
1,16,lJ
1
iW2
57,lJ
2
iW2
0,iC
21,nY
1,15,nY
2,24591,nY
1,24,iC
511,lJ
2}
,0,0x0
y7
46095,lK
46104,lK
50200,lK
287789,lK
66584,lK
407836,lK
15397,lK
62504,lK
39951,lK
24591,lK
33807,lK
62509,lK
15409,lK
50176
lF
283648
lF
19456
lF
27648
lF
90112
lF
86016
lF
190464,eJ1
eF3
195584
lF
196608,eJ1
0x14
y7
482304
lF
14342
lF
58375
lF
46147
nG
46151
lF
284679
lF
50183
lF
7183
lF
46157
nG
38991
nG
50278
lF
50279
lF
50257
lF
50258
lF
46192
lF
50295
lF
283809
lF
284835
lF
24822
lF
10240
lF
11264
lF
7170
lF
7168
lF
17408
lF
197632
lF
470016
lF
25607
lF
121871
lF
39368
lF
7192
lF
121887
lF
416811
lF
252979
lF
50262
lF
46154
lF
38919
lF
62
lF
50280
lF
40049
nG
7566,eJ1
0x10
y7
415787
lF
416819
lF
50590
nG
50590
lF
50648
nG
50648
lF
16384
lF
31746
lF
15
lF
24,eJ1
0x6
y7
24
lF
62,eJ1
0x6
y7
40048
lF
46193
nG
43
lF
43
nG
415795
lF
51
lF
51
nG
50266
lF
50176
nG
50267
lF
39159
lF
39183
nG
7168
nG
31744
lF
101376
lF
109592
lF
39022
xA2
39027
xA2
62853
lF
39326
nG
39326
lF
39332
xA2
39333,eJ1
0x1
y7
39338
nG
39338
lF
39335
xA2
15786
nG
146858
lF
39366
lF
39373
lF
39374
lF
39384
nG
39403
nG
39405
nG
39405
lF
39414
lF
39414
nG
39416
lF
15360
lF
15,eJ1
0x1
y7
16
lF
7183,eJ1
0x1
y7
7172,cPow,xZ1,lP1,0x0
y7
24591,cPow
yG3
63521,cPow
yG3
62500,cPow
yG3
50453,cPow
yG3
50200,cPow
yG3
62488,cPow,yH3
eA3
7,eA3
196,eA3
0,cAcos,x6
0,cAcosh,x6
0,cAsin,x6
0,cAsinh,x6
119,cAsinh,x6
0,cAtan,lR3
308224,cAtan2,nZ3
cAtan2,x6
0,cAtanh,x6
246,cCeil,x6
0,cCeil,eB3
yV2
0,cCos,eT
1,7,yV2
81,yV2
82,yV2
119,yV2
241,yV2
255,yV2
214,iO2
241,iO2
458,iO2
0,cCosh,eB3
iO2
246,cFloor,x6
0,cFloor,lA
e93
311587,eE3
lR3
323875,eE3
lR3
323899,eE3
l0
3,32513024,eG2
34627584
xN1
31493120,eG2
89213952
xN1
149042176
xN1
62933514
xN1
62933514,eG2
62933520
xN1
249797632
xN1
301234176
xN1
488062976
xN1
492261376
xN1
62933520,eG2
24664064
xN1
573080576
xN1
567286784
xN1
32513024
xN1
559963136
xN1
7894016
xN1
582524928,cIf,x6
119,cInt,x6
246,tA2
0,tA2
7,tA2
31,tA2
196,tA2
349,tA2
15,cLog,lT
1,24,cLog,yH3
cLog10,x6
0,cLog2,nZ3
cMax,lR3
35847,cMax,lR3
30751,cMax,x6
0,cMax,AnyParams,1
e63
y7
7168,cMin,lR3
35847,cMin,lR3
30751,cMin,x6
0,cMin,AnyParams,1
e63
y7
24591,cMin,yH3
nH2
7,nH2
81,nH2
82,nH2
119,nH2
149,nH2
254,nH2
0,cSin,eT
1,235,cSin,lA
0x5}
,{{1,246,nH2
255,nH2
273,cSin,lA
0x1}
,{{1,214,y12
235,cSinh,lA
0x5}
,{{1,246,y12
254,y12
255,y12
458,y12
0,cSinh,eB3
y12
15,cSqrt,yH3
yW2
0,cTan,eT
1,117,cTan,eT
1,118,yW2
235,yW2
246,yW2
273,yW2
254,yW2
255,yW2
0
xB2
0,cTanh,eT
1,213
xB2
235
xB2
246
xB2
254
xB2
255
xB2
0,cTrunc,lR3
15384,cSub
yG3
15384,cDiv
yG3
470476,cDiv
yG3
121913,tB2
nZ3
tB2
lA
eF3
7168,tB2
lR3
31744,tB2
lA
0x20
y7
31751,tB2
lA
0x24
y7
31751,tB2
lR3
121913,t81,nZ3
cLess,lA
eF3
41984,cLess,lA
e93
41984,cLess,lR3
7,cLess,nZ3
cLessOrEq,lR3
295158,cLessOrEq,nZ3
eI2
lA
eF3
41984,eI2
lA
e93
41984,eI2
lR3
7,eI2
nZ3
cGreaterOrEq,lR3
295158,cGreaterOrEq,x6
0
tT2
215
tT2
7
tT2
544
tT2
547
tT2
548
tT2
550
tT2
31
tT2
553
tT2
15
tT2
554,cNot,lR3
7700
lP3
7168
lP3
35847
lP3
30751
lP3
457759
lP3
460831,cAnd,i5
0,0,cAnd,nH
y02
c23
7700,c23
35847,c23
457759,c23
460831,c23
30751,cOr,i5
1,0,l62
81,l62
135,l62
215,l62
216,l62
246,cDeg,x6
246,cRad,nZ3
cAbsAnd,l6
7168,cAbsOr,i5
1,0,yL3,x6
0,cAbsNotNot,l0
3,32513024,y43
lA
0x0}
,}
;}
l63
l21{const
Rule
grammar_rules[260]={{ProduceNewTree,1,1,0,{1,0,cAbs,yY
409,{1,146,cAtan,yY
403,{2,1326,cAtan2,yY
405,{2,309249,cAtan2
cT
253174,{2,255224,cAtan2
cT
259324,{2,257274,cAtan2,yY
152,{1,252,cCeil,yY
482,{1,68
eJ2
476,{1,122
eJ2
477,{1,124
eJ2
151,{1,125
eJ2
419,{1,123
eJ2
0,{1,403,cCos,y13
0,{1,400,cCos,y13
246,{1,252
eJ2
303,{1,406,cCosh,y13
0,{1,400,cCosh,y13
246,{1,252,cCosh,yY
150,{1,252,cFloor,yY
452,{1,121,cFloor,eG3
157,{3,7382016,c1
567,{3,32513026,c1
543,{3,8430592,c1
550,{3,8436736,c1
158,{3,42998784,c1
544,{3,42999808,c1
556,{3,43039744,c1
551,{3,49291264,c1
532,{3,49325056,c1
463,{3,1058312,c1
467,{3,1058318,c1
467,{3,9438728,c1
463,{3,9438734,c1
509,{3,449213961,c1
509,{3,433500687,cIf,l2
0,3,32542219,{3,36732428,cIf,l2
0,3,32542225,{3,36732434,cIf,yY
76,{1,256,eI3
69,{1,258,eI3
404,{1,72,eI3
160,{1,147,cLog,l2
0,1,442,{eH3
cMax
eJ3
0,{2,481281,cMax
eJ3
443,{eH3
cMin
eJ3
0,{2,477185,cMin,AnyParams,0}
}
i2
0,1,154,{2,24832,cPow,eG3
154,{2,25854,cPow,eG3
155,{2,129039,cPow
n91
32062,cPow
n91
32063,cPow
n91
32064,cPow,c83
166288,{2,32137,cPow
n91
33089,cPow,l2
0,y02{2,12688,cPow,c83
7434,{2,12553
e82
429,{2,46146
e82
430,{2,46153
e82
431,{2,46150
e82
173,{2,81935
e82
172,{2,130082
e82
178,{2,133154,cPow
cT
470016,{2,464911,cPow
cT
274432,{2,273423,cPow
cT
251904,{2,266274,cPow
cT
251904,{2,263186
e82
175,{1,252,yX2
421,{1,68,yX2
151,{1,122,yX2
419,{1,124,yX2
174,{1,125,yX2
476,{1,123,yX2
0,{1,405,yX2
176,{1,252,cSinh,yY
333,{1,404,cSinh,yY
177,{1,252,eL3
0,{1,408,eL3
179,{1,410,eL3
180,{1,252,cTanh,l2
0,1,436,{2,443407,tR
1,435,{eH3
tR
1,171,{2,268549,tR
1,184,{2,276749,tR
1,183,{2,276500,tR
2,59701,{2,303289,tR
2,59702,{2,305339,tR
2,59728,{2,346306,tR
2,157004,{2,193724,tR
2,174245,{2,171172,tR
2,247974,{2,194724
i1
0}
}
i2
2,1,340,{1,313
i1
1}
}
i2
2,1,330,{1,351
i1
1}
}
,{ReplaceParams,2,1,355,{2,1376
nC
339,{2,1361
nC
457,{2,466374
nC
47,{2,349527
nC
342,{2,203823
nC
344,{2,202799
nC
474,{2,207053
nC
475,{2,208077
nC
417,{2,211144
nC
208,{2,211145
nC
418,{2,214216
nC
210,{2,202076
nC
204,{2,358748
nC
212,{2,358611
nC
222,{2,223449
nC
224,{2,228569
nC
356,{2,502185
nC
219,{2,501981
nC
226,{2,225705
nC
225,{2,225770
nC
353,{2,234921
nC
eM3
365797
nC
eN3
365789
nC
eN3
232873
nC
eM3
362986
nC
226,{2,362717,cAdd
l3
2,2,406761,{2,237775,cAdd
l3
2,2,59752,{2,237927,tR
1,93,{2,1155,c33
94,{2,1156,c33
361,{2,1386,c33
97,{2,24705,c33
98,{2,24712,c33
441,{2,443407,c33
440,{eH3
c33
402,{2,411024,c33
101,{2,102763,c33
106,{2,107848,c33
92,{2,110920,cN
2,58768,{2,1462,cN
2,15760,{2,1468,cN
2,108559,{2,97608,cN
2,94223,{2,98632,cMul
l3
1,1,0,{1,400,cMul
l3
1,1,57,{1,14,lJ
0}
}
i2
2,1,114,{2,113682,cMul,SelectedParams,0}
}
i2
2,1,561,{1,52,lJ
1}
}
i2
2,1,562,{1,42,lJ
1}
}
,{ReplaceParams,2,1,482,{2,489510
nU
488,{2,498726
nU
386,{2,435583
nU
eN3
435703
nU
eM3
502147
nU
414,{2,490880
nU
493,{2,379359
nU
371,{2,391662
nU
385,{2,425342
nU
472,{2,425454
nU
47,{2,506351
nU
499,{2,382435
nU
47,{2,510448
nU
501,{2,512038
nU
502,{2,383466
nU
375,{2,392695
nU
505,{2,385509
nU
497,{2,381408
nU
508,{2,519206
nU
504,{2,376941
nU
388,{2,375924
nU
461,{2,45510
nU
380,{2,51579
nU
462,{2,49606
nU
381,{2,47483,cMul
l3
2,2,387574,{2,386538,cN
2,41984,{2,409641,tB2
yB
tB2
yC
tB2
yE
cEqual
lS1
24849,cEqual
cT
tJ
cEqual
cT
eK1
281873,tB2
l2
cS
cEqual
cT
iU
tB2
c71
41,tB2
eO3
0,{2,5167,tB2
c83
41984,{2,409641,t81,yB
t81,yC
t81,yE
t81
lS1
24849,t81
cT
tJ
t81
cT
eK1
281873,t81,l2
cS
t81
cT
iU
t81
i71
41,t81,c71
5167,t81,yB
cLess,yC
cLess,yE
cLess,yY
565,{2,46080,cLess
lS1
24832,cLess
cT
xP1
cLess
cT
tJ
cLess
cT
eK1
y23
cLess
cT
nK1
cLess,l2
cS
cLess
cT
iU
cLess
i71
409641,cLess,yB
cLessOrEq,yC
cLessOrEq,yE
cLessOrEq,yY
559,{2,409615,cLessOrEq
lS1
24832,cLessOrEq
cT
xP1
cLessOrEq
cT
tJ
cLessOrEq
cT
eK1
y23
cLessOrEq
cT
nK1
cLessOrEq,l2
cS
cLessOrEq
cT
iU
cLessOrEq
i71
409647,cLessOrEq,yB
eI2
yC
eI2
yE
eI2
yY
533,{2,409615,cGreater
lS1
24832,cGreater
cT
xP1
cGreater
cT
tJ
cGreater
cT
eK1
y23
cGreater
cT
nK1
eI2
l2
cS
cGreater
cT
iU
eI2
c71
409647,eI2
yB
cGreaterOrEq,yC
cGreaterOrEq,yE
cGreaterOrEq,yY
566,{2,46080,cGreaterOrEq
lS1
24832,cGreaterOrEq
cT
xP1
cGreaterOrEq
cT
tJ
cGreaterOrEq
cT
eK1
y23
cGreaterOrEq
cT
nK1
cGreaterOrEq,l2
cS
cGreaterOrEq
cT
iU
cGreaterOrEq,c71
409641,cGreaterOrEq,eG3
565,{1,2,cNot,eO3
513,{1,137,cNot,eG3
560,{1,5,cAnd,AnyParams,1}
}
,{ReplaceParams,0,1,563,{2,13314,cAnd
eJ3
446,{eH3
cAnd
eJ3
538,{2,547348,cAnd
eJ3
540,{2,456219,cAnd
eJ3
542,{2,460315,cAnd
l3
0,2,530947,{3,541595138,cAnd
eJ3
564,{2,13314,yX1
557,{2,8197,yX1
451,{eH3
yX1
535,{2,547348,yX1
536,{2,456219,yX1
537,{2,460315,yX1
558,{2,143365,cOr,AnyParams,0}
}
i2
0,1,566,{1,2,y33
l4
1,1,0,{1,0,y33
eO3
519,{1,137,y33
yY
531,{1,256,cAbsNotNot,AnyParams,0}
}
i2
2,1,525,{1,254,cAbsNotNot,AnyParams,0}
}
i2
0,1,448,{3,32513580,y43
eG3
566,{3,43039744,y43
eG3
565,{3,49325056,y43
l2
0,3,32542219,{3,36732428,y43
xZ1}
,}
;cN2
grammar_optimize_abslogical_type{cQ
9
eG
grammar_optimize_abslogical_type
grammar_optimize_abslogical={9,{21,190,226,235,238,244,251,0,3}
}
;}
cN2
grammar_optimize_ignore_if_sideeffects_type{cQ
59
eG
grammar_optimize_ignore_if_sideeffects_type
grammar_optimize_ignore_if_sideeffects={59,{0,20,22,23,24,25,26,27,cW
tQ1
78,cY
cR
grammar_optimize_nonshortcut_logical_evaluation_type{cQ
56
eG
grammar_optimize_nonshortcut_logical_evaluation_type
grammar_optimize_nonshortcut_logical_evaluation={56,{0,26,cW
tQ1
78,cY
165,174,175,176,185,186,198,202,210,214,222,234,236,237,239,240,241,242,243,245,246,247,248,249,250,252,253,254,255,1,2}
}
;}
cN2
grammar_optimize_round1_type{cQ
125
eG
grammar_optimize_round1_type
grammar_optimize_round1={125,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,18,26,cW
37,38,tQ1
45,46,47,48,49,50,51,52,53,54,58,59,60,61,62,63,64,65,66,67,68,69,70,71,78,79,80,81,82,83,84,85,86,87,88,93,94,95,96,97,98,99,100,101,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,139,cR
grammar_optimize_round2_type{cQ
103
eG
grammar_optimize_round2_type
grammar_optimize_round2={103,{0,15,16,17,26,cW
39,40,tQ1
45,46,47,48,49,50,51,52,53,54,59,60,72,73,78,79,83,84,85,89,90,91,92,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,119,120,121,124,125,126,127,128,129,130,131,132,133,134,135,140,164,cR
grammar_optimize_round3_type{cQ
79
eG
grammar_optimize_round3_type
grammar_optimize_round3={79,{74,75,76,77,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,166,167,168,169,170,171,172,173,177,178,179,180,181,182,183,184,187,188,189,191,192,193,194,195,196,197,199,200,201,203,204,205,206,207,208,209,211,212,213,215,216,217,218,219,220,221,223,224,225,227,228,229,230,231,232,233}
}
;}
cN2
grammar_optimize_round4_type{cQ
10
eG
grammar_optimize_round4_type
grammar_optimize_round4={10,{19,55,56,57,137,138,160,161,162,163}
}
;}
cN2
grammar_optimize_shortcut_logical_evaluation_type{cQ
53
eG
grammar_optimize_shortcut_logical_evaluation_type
grammar_optimize_shortcut_logical_evaluation={53,{0,26,cW
tQ1
78,cY
165,174,175,176,185,186,198,202,210,214,222,234,236,239,240,241,242,243,246,247,248,249,252,253,254,255,1,2}
}
;}
}
l63
l21{eQ1
cW2
ParamSpec_Extract
iK
paramlist
cQ2
index){index=(paramlist>>(index*10))&1023;if(index>=57)return
cW2(SubFunction,yY2
plist_s[index-57]);if(index>=37)return
cW2(NumConstant,yY2
plist_n_container<yW>::plist_n[index-37])xD1
cW2(ParamHolder,yY2
plist_p[index]);}
}
#ifdef FP_SUPPORT_OPTIMIZER
#include <stdio.h>
#include <algorithm>
#include <map>
#include <sstream>
using
l63
FUNCTIONPARSERTYPES;using
l63
l21;using
t4;using
y21;l63{n41
It,typename
T,typename
Comp>eL1
MyEqualRange(It
first,It
last,const
T&val,Comp
comp){yU3
len=last-first;while(len>0){yU3
x83
len/2;It
nM3(first);nM3+=half;if(comp(*nM3,val)){first=nM3;++first;len=len-half-1;}
nC2
comp(val,*nM3)){len=half;}
else{It
left(first);{It&eK2=left;It
last2(nM3);yU3
len2=last2-eK2;while(len2>0){yU3
half2=len2/2;It
tP3(eK2);tP3+=half2;if(comp(*tP3,val)){eK2=tP3;++eK2;len2=len2-half2-1;}
else
len2=half2;}
}
first+=len;It
right(++nM3);{It&eK2=right;It&last2=first;yU3
len2=last2-eK2;while(len2>0){yU3
half2=len2/2;It
tP3(eK2);tP3+=half2;if(comp(val,*tP3))len2=half2;else{eK2=tP3;++eK2;len2=len2-half2-1;}
}
tD3
eL1(left,right);}
tD3
eL1(first,first);}
eQ1
cN2
OpcodeRuleCompare{iC2()(x53
cQ2
y22
tM3{const
Rule&rule=grammar_rules[y22]xD1
tree
nE<rule
nQ2.subfunc_opcode;}
iC2()iK
y22,x53
tM3{const
Rule&rule=grammar_rules[y22]xD1
rule
nQ2.subfunc_opcode<tree
nE;}
}
nI1
bool
TestRuleAndApplyIfMatch
nD2
xA&tree,bool
c9{MatchInfo<yW>info;lK1
found(false,e1());if((rule.eM1
LogicalContextOnly)&&!c9{c22
if(nD
IsIntType<yW>::cG3){if(rule.eM1
NotForIntegers)c22
tC3(rule.eM1
OnlyForIntegers)c22
for(;;){
#ifdef DEBUG_SUBSTITUTIONS
#endif
found=TestParams(rule
nQ2,tree,found.specs,info,true);if(found.found)break;if(!&*found.specs){fail:;
#ifdef DEBUG_SUBSTITUTIONS
DumpMatch
nF2,false);
#endif
return
e03}
#ifdef DEBUG_SUBSTITUTIONS
DumpMatch
nF2,true);
#endif
SynthesizeRule
nF2)xA1}
y21{x61
ApplyGrammar
n92
Grammar&tC2,xA&tree,bool
c9{if
iH.GetOptimizedUsing()==&tC2){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Already optimized:  "
;DumpTree
iH)xL1"\n"
<<std::flush;
#endif
return
e03
if(true){bool
changed=false;switch
iH
nE
iK2
cNot:case
cNotNot:case
cAnd:case
cOr:e22
tree.x8
true))nP2
lD
cIf:case
cAbsIf:if(ApplyGrammar(tC2,i42,eP2
cIf))nP2
for
e51
1;a<tree.x8
c9)nP2
break;default:e22
tree.x8
false))nP2}
if(changed){tree.Mark_Incompletely_Hashed()xA1}
typedef
const
unsigned
char*lS3;std::pair<lS3,lS3>range=MyEqualRange(tC2.rule_list,tC2.rule_list+tC2.rule_count,tree,OpcodeRuleCompare
t63
e52
unsigned
char>rules;rules.x63
range.second-range.first);for
y4
if(IsLogisticallyPlausibleParamsMatch(cZ1
nQ2,tree))rules
i0*r);}
range.first=!rules
i23?&rules[0]:0;range.second=!rules
i23?&rules[rules.yD3]+1:0;if(range.first!=range.second){
#ifdef DEBUG_SUBSTITUTIONS
if(range.first!=range.second)y32"Input ("
<<FP_GetOpcodeName
iH
nE)<<")["
<<tree
cB1<<"]"
;if(c9
l13<<"(Logical)"
;unsigned
first=tR1,prev=tR1;l33
sep=", rules "
;for
y4
if(first==tR1)first=prev=*r;nC2*r==prev+1)prev=*r;else
y32
sep<<first;sep=","
;if(prev!=first)l13<<'-'<<prev;first=prev=*r;}
}
if(first!=tR1)y32
sep<<first;if(prev!=first)l13<<'-'<<prev;}
l13<<": "
;DumpTree
iH)xL1"\n"
<<std::flush;}
#endif
bool
changed=false;for
y4
#ifndef DEBUG_SUBSTITUTIONS
if(!IsLogisticallyPlausibleParamsMatch(cZ1
nQ2,tree))y01
#endif
if(TestRuleAndApplyIfMatch(cZ1,tree,c9){nP2
y93
if(changed){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Changed."
<<std::endl
xL1"Output: "
;DumpTree
iH)xL1"\n"
<<std::flush;
#endif
tree.Mark_Incompletely_Hashed()xA1}
tree.SetOptimizedUsing(&tC2)xD1
e03
e01
ApplyGrammars(FPoptimizer_CodeTree::cZ2
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_round1\n"
;
#endif
n3
grammar_optimize_round1
x2
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_round2\n"
;
#endif
n3
grammar_optimize_round2
x2
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_round3\n"
;
#endif
n3
grammar_optimize_round3
x2
#ifndef FP_ENABLE_SHORTCUT_LOGICAL_EVALUATION
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_nonshortcut_logical_evaluation\n"
;
#endif
n3
grammar_optimize_nonshortcut_logical_evaluation
x2
#endif
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_round4\n"
;
#endif
n3
grammar_optimize_round4
x2
#ifdef FP_ENABLE_SHORTCUT_LOGICAL_EVALUATION
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_shortcut_logical_evaluation\n"
;
#endif
n3
grammar_optimize_shortcut_logical_evaluation
x2
#endif
#ifdef FP_ENABLE_IGNORE_IF_SIDEEFFECTS
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_ignore_if_sideeffects\n"
;
#endif
n3
grammar_optimize_ignore_if_sideeffects
x2
#endif
#ifdef DEBUG_SUBSTITUTIONS
l13<<i83"grammar_optimize_abslogical\n"
;
#endif
n3
grammar_optimize_abslogical
x2
#undef C
}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
#include <algorithm>
#include <assert.h>
#include <cstring>
#include <cmath>
#include <memory> /* for auto_ptr */
using
l63
FUNCTIONPARSERTYPES;using
l63
l21;using
t4;using
y21;l63{x61
TestImmedConstraints
iK
bitmask,x53){switch(bitmask&ValueMask
iK2
Value_AnyNum:case
ValueMask:lD
nG2:if(GetEvennessInfo
iH)!=l72
Value_OddInt:if(GetEvennessInfo
iH)!=nI2
t51:if(GetIntegerInfo
iH)!=l72
Value_NonInteger:if(GetIntegerInfo
iH)!=nI2
nA2:if(!IsLogicalValue
iH)y5
lT1
SignMask
iK2
Sign_AnySign:lD
lP1:if(lC1
l72
eI1:if(lC1
nI2
Sign_NoIdea:if(lC1
Unknown
y5
lT1
OnenessMask
iK2
Oneness_Any:case
OnenessMask:lD
Oneness_One:if(!c32
if(!lI3
fp_abs
iH
nJ1),yW(1))y5
lD
Oneness_NotOne:if(!c32
iD1
fp_abs
iH
nJ1),yW(1))y5
lT1
ConstnessMask
iK2
Constness_Any:lD
nZ2:if(!c32
lD
Constness_NotConst:if
iH
xF1
y5
break;}
tW1}
eT1
unsigned
extent
cQ2
nbits,typename
eL2=unsigned
int>cN2
nbitmap{private:static
const
unsigned
bits_in_char=8;static
const
unsigned
eM2=(y53
eL2)*bits_in_char)/nbits;eL2
data[(extent+eM2-1)/eM2];cI3
void
inc
iK
index,int
by=1){data[pos(index)]+=by*eL2(1<<y42);iM
void
dec
iK
index){inc(index,-1);}
int
get
iK
index
yJ1(data[pos(index)]>>y42)&mask()e92
pos
c42
index/eM2
e92
shift
c42
nbits*(index%eM2)e92
mask()yZ(1<<nbits)-1
e92
mask
c42
mask()<<y42;}
}
;cN2
yT3{int
SubTrees:8;int
Others:8;int
lF2:8;int
cL3:8;nbitmap<iN2,2>SubTreesDetail;yT3(){std::memset(this,0,y53*this));}
yT3
n92
yT3&b){std::memcpy(this,&b,y53
b));}
yT3&cS1=n92
yT3&b){std::memcpy(this,&b,y53
b))xD1*this;}
}
nI1
yT3
CreateNeedList_uncached(eU&tD2{yT3
xM1;for
iK
a=0;a<c43
y52;++a){const
cW2&lT3=lU1
c43.param_list,a);xT3
SubFunction:{const
xH&n82
xH*xJ1;yI
GroupFunction)++xM1.cL3;else{++e42;assert(iC3.subfunc_opcode<VarBegin);xM1.SubTreesDetail.inc
iS3
xE1);}
++xM1.lF2;break
cS2
NumConstant:case
ParamHolder:++e32;++xM1.lF2;y93
return
xM1;}
eQ1
yT3&CreateNeedList(eU&tD2{typedef
std::map<eU*,yT3>cN1;static
cN1
yE1;cN1::xN3
i=yE1.xI2&tD2;if(i!=yE1
yQ1&tD2
return
i->second
xD1
yE1.xK3,std::make_pair(&c43,CreateNeedList_uncached<yW>(tD2))->second;}
eQ1
xA
CalculateGroupFunction
n92
cW2&y62
const
iQ3){xT3
NumConstant:{const
eF&n82
eF*xJ1
xD1
CodeTreeImmed
iS3
cC2)cS2
n72
y03&n82
xG*xJ1
xD1
info.GetParamHolderValueIfFound
iS3.index)xK1
const
xH&n82
xH*xJ1;xA
cG3;cG3
cC
param
xE1);cG3.iG1).x63
iC3
y52);for
iK
a=0;a<iC3
y52;++a
lG
tmp(CalculateGroupFunction(lU1
iC3.param_list,a),info));cG3
t11
tmp);}
cG3.Rehash()xD1
cG3;}
tD3
xA();}
}
y21{x61
IsLogisticallyPlausibleParamsMatch(eU&c43,x53){yT3
xM1(CreateNeedList<yW>(tD2);yU3
eP3=xW
if(eP3<yU3(xM1.lF2))tF2
e22
eP3;++a)cF3
opcode=xL2
nE;switch(opcode
iK2
cImmed:if(xM1.cL3>0)--xM1.cL3;else--e32;lD
iN2:case
cFCall:case
cPCall:--e32;break;default:assert(opcode<VarBegin);if(e42>0&&xM1.SubTreesDetail.get(opcode)>0){--e42;xM1.SubTreesDetail.dec(opcode);}
else--e32;}
}
if(xM1.cL3>0||e42>0||e32>0)tF2
if(c43.match_type!=AnyParams){if(0||e42<0||e32<0)tF2}
tW1}
eQ1
lK1
TestParam
n92
cW2&y62
x53
lM3
iB2,iQ3){xT3
NumConstant:{const
eF&n82
eF*xJ1;if(!c32
yW
imm=tree
nJ1;switch
iS3.modulo
iK2
Modulo_None:lD
Modulo_Radians:imm=fp_mod(imm,cD
imm
t53
imm
cF
if(imm>fp_const_pi
t63
imm-=fp_const_twopi<yW>(xC2
return
lI3
imm,param
cC2)cS2
n72
y03&n82
xG*xJ1;if(!x1
return
info.SaveOrTestParamHolder
iS3.index,tree)xK1
const
xH&n82
xH*xJ1;yI
GroupFunction){if(!x1
xA
xQ1=CalculateGroupFunction(y62
info);
#ifdef DEBUG_SUBSTITUTIONS
DumpHashes(xQ1)xL1*n92
void**)&xQ1
nJ1
xL1"\n"
xL1*n92
void**)&tree
nJ1
xL1"\n"
;DumpHashes
iH)xL1"Comparing "
;lX3
xQ1)xL1" and "
;DumpTree
iH)xL1": "
xL1(xQ1
xB
tree)?"true"
:"false"
)xL1"\n"
;
#endif
return
xQ1
xB
tree);}
tC3(!&*iB2){if(!x1
if
iH
nE!=param
xE1
y5
tD3
TestParams(iC3,tree,iB2,info,false);}
}
tD3
e03
eQ1
cN2
iN{e1
l82
MatchInfo<yW>info;iN(tE2,info(){}
}
nI1
class
MatchPositionSpec_PositionalParams:tY1
iN<yW> >{cI3
iP2
MatchPositionSpec_PositionalParams(y73
y11
iN<yW> >(n){}
}
;cN2
tS1{e1
l82
tS1(tE2{}
}
;class
yJ:tY1
tS1>{cI3
unsigned
trypos;iP2
yJ(y73
y11
tS1>(n),trypos(0){}
}
nI1
lK1
TestParam_AnyWhere
n92
cW2&y62
x53
lM3
iB2,iQ3,std::vector<bool>&used,bool
xZ2{xR<yJ>x7;unsigned
lZ3
yJ*)&*l82
a=x7->trypos;goto
retry_anywhere_2
c52
yJ
iH
cB1);a=0;}
eQ3
xW++a){if(used[a])y01
retry_anywhere:{lK1
r=TestParam(y62
xL2,cH3);(eR3
used[a]=true;if(xZ2
iR3
a);x7->trypos=a
xD1
lK1(true,&*x7);}
}
retry_anywhere_2:if(eS3
goto
retry_anywhere;}
tD3
e03
eQ1
cN2
y31{e1
l82
MatchInfo<yW>info
e52
bool>used;iP2
y31(yU3
eP3
tE2,info(),used(eP3){}
}
nI1
class
MatchPositionSpec_AnyParams:tY1
y31<yW> >{cI3
iP2
MatchPositionSpec_AnyParams(y73,yU3
m
y11
y31<yW> >(n,y31<yW>(m)){}
}
nI1
lK1
TestParams(eU&nP,x53
lM3
iB2,iQ3,bool
xZ2{if(nP.match_type!=AnyParams){if(y2!=tree
cB1
y5}
if(!IsLogisticallyPlausibleParamsMatch(nP,tree))tF2
switch(nP.match_type
iK2
PositionalParams:{xR<yF>x7;unsigned
lZ3
yF*)&*l82
a=y2-1;goto
l51
c52
yF(y2);a=0;}
eQ3
y2;++a){(*x7)[a].tH2
retry_positionalparams:{lK1
r=TestParam(yK
a),xL2,cH3);(eR3
y01}
}
l51:if(eS3
info
lU3.info;goto
retry_positionalparams;}
if(a>0){--a;goto
l51;}
info=(*x7)[0].info
xD1
e03
if(xZ2
for
iK
a=0;a<y2;++a)iR3
a)xD1
lK1(true,&*x7)cS2
SelectedParams:case
AnyParams:{xR<eV>x7
e52
bool>used
iH
cB1)e52
unsigned>iQ2(y2)e52
unsigned>y72(y2)iJ{const
cW2
lT3=yK
a);iQ2[a]=ParamSpec_GetDepCode(lT3);}
cF3
b=0
iJ
if(iQ2[a]!=0)y72[b++]=a
iJ
if(iQ2[a]==0)y72[b++]=a;}
unsigned
lZ3
eV*)&*l82
if(y2==0){a=0;goto
retry_anyparams_4;}
a=y2-1;goto
cO1
c52
eV(y2,tree
cB1);a=0;if(y2!=0){(*x7)[0].tH2(*x7)[0].used=used;}
}
eQ3
y2;++a){if(a>0){(*x7)[a].tH2(*x7)[a].used=used;}
retry_anyparams:{lK1
r=TestParam_AnyWhere<yW>(yK
y72[a]),tree,cH3,used,xZ2;(eR3
y01}
}
cO1:if(eS3
info
lU3.info;used
lU3.used;goto
retry_anyparams;}
cP1:if(a>0){--a;goto
cO1;}
info=(*x7)[0].info
xD1
e03
retry_anyparams_4:if(nP.n1!=0){if(!TopLevel||!info.HasRestHolder(nP.n1)){t01
c62;c62.reserve
iH
cB1);for
iK
b=0;b<xW++b){if(y83)y01
c62
i0
tree
l7
b));y83=true;if(xZ2
iR3
b);}
if(!info.SaveOrTestRestHolder(nP.n1,c62)){goto
cP1;}
}
else{nX3
c62=info.GetRestHolderValues(nP.n1)xG1
c62
iT2
a){bool
found=false;for
iK
b=0;b<xW++b){if(y83)y01
if(c62[a]xB
tree
l7
b))){y83=true;if(xZ2
iR3
b);found=true;y93
if(!found){goto
cP1;}
}
}
tD3
lK1(true,y2?&*x7:0)cS2
GroupFunction:break;tD3
e03}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
#include <algorithm>
#include <assert.h>
using
t4;using
y21;l63{eQ1
xA
xR1
const
cW2&y62
iQ3,bool
inner=true){xT3
NumConstant:{const
eF&n82
eF*xJ1
xD1
CodeTreeImmed
iS3
cC2)cS2
n72
y03&n82
xG*xJ1
xD1
info.GetParamHolderValue
iS3.index)xK1
const
xH&n82
xH*xJ1;xA
tree;n23
param
xE1);for
iK
a=0;a<iC3
y52;++a
lG
nparam=xR1
lU1
iC3.param_list,a),info,true
n61
nparam);}
xZ3
xU3!=0){t01
trees(info.GetRestHolderValues
iS3
xU3));tree.AddParamsMove(trees);if
iH
yU1
1){assert(tree.GetOpcode()==cAdd i73()==cMul i73()==cMin i73()==cMax i73()==cAnd i73()==cOr i73()==cAbsAnd i73()==cAbsOr);tree
t31
0));}
else
if
iH
yU1
0){switch
iH
nE
iK2
cAdd:case
cOr:tree=cH1
0));lD
cMul:case
cAnd:tree=cH1
1));default:y93}
if(inner)tree.Rehash()xD1
tree;}
tD3
xA();}
}
y21{e01
SynthesizeRule
nD2
xA&tree,iQ3){switch(rule.ruletype
iK2
ProduceNewTree:{tree.Become(xR1
lU1
rule.eN1
0),info,false)xC2
case
ReplaceParams:default:{std::vector<unsigned>list=info.GetMatchedParamIndexes();std::sort(list.iH2
list
tF3)xR3
list.size();a-->0;)tree.iY1
list[a]);for
iK
a=0;a<rule.repl_param_count;++a
lG
nparam=xR1
lU1
rule.eN1
a),info,true
n61
nparam);}
y93}
}
#endif
#ifdef DEBUG_SUBSTITUTIONS
#include <sstream>
#include <cstring>
using
l63
FUNCTIONPARSERTYPES;using
l63
l21;using
t4;using
y21;l63
l21{e01
DumpMatch
nD2
x53,const
iQ3,bool
DidMatch,std::ostream&o){DumpMatch
nF2,DidMatch?iM3"match"
:iM3"mismatch"
,o);}
e01
DumpMatch
nD2
x53,const
iQ3,l33
eT3,std::ostream&o){static
const
char
ParamHolderNames[][2]={"%"
,"&"
,"x"
,"y"
,"z"
,"a"
,"b"
,"c"
}
;o<<eT3<<" (rule "
<<(&rule-grammar_rules)<<")"
<<":\n  Pattern    : "
;{cW2
tmp;tmp.first=SubFunction;xH
tmp2;tmp2.data=rule
nQ2;tmp.second=yY2
tmp2;DumpParam<yW>(tmp,o);}
o<<"\n  Replacement: "
;DumpParams<yW>(rule.eN1
rule.repl_param_count
eV3
o<<"  Tree       : "
;DumpTree
iH
eV3
if(!std::strcmp(eT3,iM3"match"
))DumpHashes
iH,o)xG1
info.y6
iT2
a){if(!info.y6[a].lZ2)y01
o<<"           "
<<ParamHolderNames[a]<<" = "
;lX3
info.y6[a]eV3}
iM1
info.lO
iT2
b){if(!tI2
first)continue
xG1
tI2
second
iT2
a){o<<"         <"
<<b<<"> = "
;lX3
tI2
second[a]eU3
l53}
}
o<<std::flush;}
}
#endif
#include <list>
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63{x61
MarkIncompletes(FPoptimizer_CodeTree::cZ2
if
iH.Is_Incompletely_Hashed())tW1
bool
tT1=false
xG1
xW++a)tT1|=MarkIncompletes
iH
l7
a));if(tT1)tree.Mark_Incompletely_Hashed()xD1
tT1;}
e01
FixIncompletes(FPoptimizer_CodeTree::cZ2
if
iH.Is_Incompletely_Hashed()){e22
xW++a)FixIncompletes
iH
l7
a));tree
tZ1}
}
}
t4{lB
Sort()eK3
Sort();}
lB
Rehash(bool
constantfolding){if(constantfolding)ConstantFolding(*this);else
Sort();data
x9
eQ1
iV
yU
const
yW&eX3
y63=0;
#if 0
long
double
value=Value;eA=crc32::calc(n92
unsigned
char*)&value,y53
value));key^=(key<<24);
#elif 0
union{cN2
cF3
char
filler1[16];yW
v;unsigned
char
filler2[16];}
buf2;cN2
cF3
char
filler3[y53
yW)+16-y53
fphash_value_t)];eA;}
buf1;}
data;memset(&data,0,y53
data));data.buf2.v=Value;eA=data.buf1.key;
#else
int
nR3;yW
lV2=std::frexp(Value,&nW2;eA=iK(nR3+0x8000)&0xFFFF);if(lV2<0){lV2=-lV2;key=key^0xFFFF;}
else
key+=0x10000;lV2-=yW(0.5);key<<=39;key|=fphash_value_t((lV2+lV2)*yW(1u<<31))<<8;
#endif
iO
lQ
#ifdef FP_SUPPORT_LONG_INT_TYPE
eT1>iV<long>yU
long
eX3
eA=Value;iO
lQ
#endif
#ifdef FP_SUPPORT_GMP_INT_TYPE
eT1>iV<GmpInt>yU
const
GmpInt&eX3
eA=Value.toInt();iO
lQ
#endif
e01
x12<yW>::Recalculate_Hash_NoRecursion(){fphash_t
iO(fphash_value_t(Opcode)<<56,Opcode*iB3(0x1131462E270012B));Depth=1;switch(Opcode
iK2
cImmed:{ImmedHashGenerator<yW>::MakeHash(iO,Value
xC2
case
iN2:{y63|=tK<<48
cU2((tK)*11)^iB3(0x3A83A83A83A83A0);break
cS2
cFCall:case
cPCall:{y63|=tK<<48
cU2((~tK)*7)^3456789;}
default:{yU3
eO1=0
xG1
yV3
iT2
a){if(yV3[a]xS2>eO1)eO1=yV3[a]xS2;y63+=((yV3[a]cT2
hash1*(a+1))>>12)cU2
yV3[a]cT2
hash1
cU2(3)*iB3(0x9ABCD801357);iO.hash2*=iB3(0xECADB912345)cU2(~yV3[a]cT2
hash2)^4567890;}
Depth+=eO1;}
}
if(Hash!=iO){Hash=iO;iJ1=0;}
}
lB
FixIncompleteHashes(){MarkIncompletes(*this);FixIncompletes(*this);}
}
#endif
#include <cmath>
#include <list>
#include <cassert>
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63{using
t4
nI1
bool
nL1
x53,long
count,const
x11::SequenceOpCode<yW>&eP,x11::n81&synth,yU3
max_bytecode_grow_length);static
const
cN2
SinCosTanDataType{OPCODE
whichopcode;OPCODE
inverse_opcode;enum{nominator,lW2,inverse_nominator,l61}
;OPCODE
codes[4];}
SinCosTanData[12]={{cTan,cCot,{cSin,cCos,cCsc,cSec}
}
,{cCot,cCot,{cCos,cSin,cSec,cCsc}
}
,{cCos,cSec,{cSin,cTan,cCsc,cCot}
}
,{cSec,cCos,{cTan,cSin,cCot,cCsc}
}
,{cSin,cCsc,{cCos,cCot,cSec,cTan}
}
,{cCsc,cSin,{cCot,cCos,cTan,cSec}
}
,{c72{cSinh,cCosh,c82,{cSinh,cNop,{c72
cNop,cCosh}
}
,{cCosh,cNop,{cSinh,c72
cNop}
}
,{cNop,cTanh,{cCosh,cSinh,c82,{cNop,cSinh,{cNop,cTanh,cCosh,cNop}
}
,{cNop,cCosh,{cTanh,cSinh,c82}
;}
t4{lB
SynthesizeByteCode(std::vector<unsigned>&iB,std::vector<yW>&Immed,yU3&stacktop_max){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Making bytecode for:\n"
;iG
#endif
while(RecreateInversionsAndNegations()){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"One change issued, produced:\n"
;iG
#endif
FixIncompleteHashes();}
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Actually synthesizing, after recreating inv/neg:\n"
;iG
#endif
x11::n81
synth;SynthesizeByteCode(synth,false);synth.Pull(iB,Immed,stacktop_max);}
lB
SynthesizeByteCode(x11::n81&synth,bool
MustPopTemps
tM3{xS1*this))yZ;}
e22
12;++a){const
SinCosTanDataType&data=SinCosTanData[a];if(data.whichopcode!=cNop)i81!=data.whichopcode)y01
CodeTree
lV3;lV3.lD1
lV3
cC
data.inverse_opcode);lV3.y82);xS1
lV3)){synth.iW
else
i81!=cInv)y01
if(GetParam(0)nE!=data.inverse_opcode)y01
xS1
GetParam(0))){synth.iW
yU3
found[4];iM1
4;++b){CodeTree
tree;if(data.eY3]==cNop){n23
cInv);CodeTree
lW3;lW3.lD1
lW3
cC
data.eY3^2]);lW3.y82
n61
lW3);}
else{tree.lD1
n23
data.eY3]);}
tree.y82);found[b]eZ3
cQ3
iH);}
if(found[data.yA2!=tA
lW2]!=iP
yA2)tG2
lW2
iQ
cDiv
lV1
yA2!=tA
l61]!=iP
yA2)tG2
l61
iQ
cMul
lV1
lF1!=tA
l61]!=iP
lF1)tG2
l61
iQ
cRDiv
lV1
lF1!=tA
lW2]!=iP
lF1)tG2
lW2
iQ
cMul,2,1);synth.iW
yU3
n_subexpressions_synthesized=SynthCommonSubExpressions(cV1;switch(iA1{case
iN2:synth.PushVar(GetVar());lD
cImmed:i31
GetImmed());lD
cAdd:case
cMul:case
cMin:case
cMax:case
cAnd:case
cOr:case
cAbsAnd:case
cAbsOr:i81==cMul){bool
yA3=false;yL
lG1
xF1&&isLongInteger(lG1
nJ1)){yM1=makeLongInteger(lG1
nJ1);CodeTree
tmp(*this,typename
CodeTree::CloneTag());tmp
i33
tmp
tZ1
if(nL1
tmp,value,x11::eX1<yW>::AddSequence,synth,MAX_MULI_BYTECODE_LENGTH)){yA3=true;y93}
if(yA3)break;}
int
y41=0
e52
bool>done(GetParamCount(),false);CodeTree
i6;i6
cC
iA1;for(;;){bool
found=false;yL
done[a])y01
if(synth.IsStackTop(lG1)){found=true;done[a]=true;lG1.n9
i6
cJ
lG1);if(++y41>1){yM
2);i6.y82
i11
i6);y41=y41-2+1;}
}
}
if(!found)break;}
yL
done[a])y01
lG1.n9
i6
cJ
lG1);if(++y41>1){yM
2);i6.y82
i11
i6);y41=y41-2+1;}
}
if(y41==0){switch(iA1{case
cAdd:case
cOr:case
cAbsOr:i31
0);lD
cMul:case
cAnd:case
cAbsAnd:i31
1);lD
cMin:case
cMax:i31
0);break;default:break;}
++y41;}
assert(n_stacked==1);break
cS2
cPow:{l73
p0
nR2(0);l73
p1
nR2(1);if(!p1
xF1||!isLongInteger
tQ3||!nL1
p0,makeLongInteger
tQ3,x11::eX1<yW>::MulSequence,synth,MAX_POWI_BYTECODE_LENGTH)){p0.n9
p1.n9
yM
2);e41
cIf:case
cAbsIf:{typename
x11::n81::IfData
nL3;GetParam(0)tH3
SynthIfStep1(nL3,iA1;GetParam(1)tH3
SynthIfStep2(nL3);GetParam(2)tH3
SynthIfStep3(nL3
xC2
case
cFCall:case
cPCall:{e22
l01++a)lG1.n9
yM
iK)GetParamCount());xF3
lP2|GetFuncNo(),0,0
xC2
default:{e22
l01++a)lG1.n9
yM
iK)GetParamCount()xC2}
synth.StackTopIs(*this);if(MustPopTemps&&n_subexpressions_synthesized>0){yU3
top
eZ3
GetStackTop();synth.DoPopNMov(top-1-n_subexpressions_synthesized,top-1);}
}
}
l63{x61
nL1
x53,long
count,const
x11::SequenceOpCode<yW>&eP,x11::n81&synth,yU3
max_bytecode_grow_length){if
yX3!=0){x11::n81
backup=synth;tree.n9
yU3
bytecodesize_backup
eZ3
GetByteCodeSize();x11::nL1
count
tT3
yU3
bytecode_grow_amount
eZ3
GetByteCodeSize()-bytecodesize_backup;if(bytecode_grow_amount>max_bytecode_grow_length){synth=backup
xD1
e03
tW1}
else{x11::nL1
count,eP,cV1
xA1}
}
#endif
#include <cmath>
#include <cassert>
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63{using
t4;
#define FactorStack std::vector
const
cN2
PowiMuliType
cF3
opcode_square;unsigned
opcode_cumulate;unsigned
opcode_invert;unsigned
opcode_half;unsigned
opcode_invhalf;}
iseq_powi={cSqr,cMul,cInv,cSqrt,cRSqrt}
,iseq_muli={tR1
y3
cNeg,tR1,tR1}
nI1
yW
c81
const
PowiMuliType&yB3,i01,i91&stack)cP3
1);while(IP<limit){if(yC3
yB3.opcode_square){if(!t52
cG3))l92
2;c5
opcode_invert){nE3-cG3;c5
opcode_half){if
yZ3>i13
isEvenInteger
yZ3))l92
yW(0.5);c5
opcode_invhalf){if
yZ3>i13
isEvenInteger
yZ3))l92
yW(-0.5);++IP;y01}
yU3
nJ2=IP;yW
lhs(1);if(yC3
cFetch)cF3
index=yJ3;if(index<y0||yU3(index-y0)>=c53
size()){IP=nJ2;break;}
lhs=stack[index-y0];goto
yB2;}
if(yC3
cDup){lhs=cG3;goto
yB2;yB2:yE3
cG3);++IP;yW
subexponent=c81
yB3
tL
if(IP>=limit||iB[IP]!=yB3.opcode_cumulate){IP=nJ2;break;}
++IP;c53
pop_back();cG3+=lhs*subexponent;y01}
break;tD3
cG3;}
eQ1
yW
ParsePowiSequence(i01){i91
stack;yE3
yW(1))xD1
c81
iseq_powi
tL}
eQ1
yW
ParseMuliSequence(i01){i91
stack;yE3
yW(1))xD1
c81
iseq_muli
tL}
eQ1
class
CodeTreeParserData{cI3
iP2
CodeTreeParserData(bool
k_powi):stack(),clones(),keep_powi(k_powi){}
void
Eat(yU3
eP3,OPCODE
opcode
lG
xI;xI
cC
opcode);t01
c43=Pop(eP3);xI
y92
tD2;if(!keep_powi)switch(opcode
iK2
cTanh:{xA
sinh,cosh;sinh
cC
cSinh);sinh
cJ
xI
l7
0));sinh
tZ1
cosh
cC
cCosh);cosh
t11
xI
l7
0));cosh
tZ1
xA
pow;pow
cC
cPow);pow
t11
cosh);pow.yG
yW(-1)));pow
tZ1
xI
x03
xI.lO1
0,sinh);xI
t11
pow
xC2
case
cTan:{xA
sin,cos;sin
cC
cSin);sin
cJ
xI
l7
0));sin
tZ1
cos
cC
cCos);cos
t11
xI
l7
0));cos
tZ1
xA
pow;pow
cC
cPow);pow
t11
cos);pow.yG
yW(-1)));pow
tZ1
xI
x03
xI.lO1
0,sin);xI
t11
pow
xC2
case
cPow:{iK1
p0=xI
l7
0);iK1
p1=xI
l7
1);if(p1
nE==cAdd){t01
mulgroup(p1
cB1)xG1
p1.l01++a
lG
pow;pow
cC
cPow);pow
cJ
p0);pow
cJ
p1
l7
a));pow
tZ1
mulgroup[a
t03
pow);}
xI
x03
xI
y92
mulgroup);}
break;}
default:break;}
xI.Rehash(!keep_powi);tU1,false);
#ifdef DEBUG_SUBSTITUTIONS
nX2<<eP3<<", "
<<FP_GetOpcodeName(opcode)<<"->"
<<FP_GetOpcodeName(xI
nE)<<": "
iN3
lX3
xI
iI(xI);
#endif
yE3
xI
cM3
EatFunc(yU3
eP3,OPCODE
opcode
cQ2
funcno
lG
xI=CodeTreeFuncOp<yW>(opcode,funcno);t01
c43=Pop(eP3);xI
y92
tD2;xI.y82);
#ifdef DEBUG_SUBSTITUTIONS
nX2<<eP3<<", "
iN3
lX3
xI
iI(xI);
#endif
tU1);yE3
xI
cM3
xT
lE2
lG
xI=CodeTreeImmed(value);tU1);Push(xI
cM3
AddVar
iK
varno
lG
xI=CodeTreeVar<yW>(varno);tU1);Push(xI
cM3
SwapLastTwoInStack(){yF3
yD3
t03
yF3
size()-2]cM3
Dup(){Fetch(c53
yD3
cM3
Fetch(yU3
which){Push(stack[which]);}
n41
T>void
Push(T
tree){
#ifdef DEBUG_SUBSTITUTIONS
l13<<iN3
DumpTree
iH
iI
iH);
#endif
yE3
tree
cM3
PopNMov(yU3
target,yU3
source){stack[target]=stack[source];stack
nY3
target+1);}
xA
PullResult(){clones.clear();xA
cA3
c53
back());stack
nY3
c53
yD3)xD1
cG3;}
t01
Pop(yU3
n_pop){t01
cA3
n_pop);for
iK
n=0;n<n_pop;++n)cG3[n
t03
yF3
size()-n_pop+n]);
#ifdef DEBUG_SUBSTITUTIONS
for(y73=n_pop;n-->0;){nX2;lX3
cG3[n]iI
yZ3[n]);}
#endif
stack
nY3
c53
size()-n_pop)xD1
cG3;}
yU3
GetStackTop(yJ1
c53
size();}
private:void
FindClone(xA&,bool=true)yZ;}
private:t01
stack;std::multimap<fphash_t,xA>clones;bool
keep_powi;private:CodeTreeParserData
n92
CodeTreeParserData&);CodeTreeParserData&cS1=n92
CodeTreeParserData&);}
nI1
cN2
IfInfo{xA
eN2;xA
thenbranch;yU3
endif_location;IfInfo():eN2(),thenbranch(),endif_location(){}
}
;}
t4{lB
GenerateFrom
n92
typename
FunctionParserBase<yW>::Data&nN3,bool
keep_powi){t01
nV2;nV2.x63
nN3.mVariablesAmount);for
iK
n=0;n<nN3.mVariablesAmount;++n){nV2
i0
CodeTreeVar<yW>(n+iN2));}
GenerateFrom(nN3,nV2,keep_powi);}
lB
GenerateFrom
n92
typename
FunctionParserBase<yW>::Data&nN3,const
x4&nV2,bool
keep_powi){xC1
unsigned>&iB=nN3.mByteCode;xC1
yW>&Immed=nN3.mImmed;
#ifdef DEBUG_SUBSTITUTIONS
l13<<"ENTERS GenerateFrom()\n"
;
#endif
CodeTreeParserData<yW>sim(keep_powi)e52
IfInfo<yW> >eN;for(yU3
IP=0,DP=0;;++IP){tJ2:while(!eN
i23&&(eN.eB==IP||(IP<iB.size()&&yC3
cJump&&eN
nM2.lZ2))){CodeTree
elsebranch=sim
tZ2
tY2
eN.back().eN2
tY2
eN
nM2
tY2
elsebranch)yN
3,cIf);eN.pop_back();}
if(IP>=iB.size())break;unsigned
opcode=iB[IP];if((opcode==cSqr||opcode==cDup||(opcode==cInv&&!IsIntType<yW>::cG3)||opcode==cNeg||opcode==cSqrt||opcode==cRSqrt||opcode==cFetch)){yU3
was_ip=IP;yW
eB2
ParsePowiSequence<yW>(iB,IP,eN
i23?iB.size():eN.eB,sim.xL
1);if(nR3!=yW(1.0)){x82
nW2
yN
2
c63;goto
tJ2;}
if(opcode==cDup||opcode==cFetch||opcode==cNeg){yW
yS2=ParseMuliSequence<yW>(iB,IP,eN
i23?iB.size():eN.eB,sim.xL
1);if
nL2!=yW(1.0)){x82
yS2)yN
2,cMul);goto
tJ2;}
}
IP=was_ip;}
if(lA2>=iN2)cF3
index=opcode-iN2;sim.Push(nV2[index]);}
else{switch(lA2
iK2
cIf:case
cAbsIf:{eN
nY3
eN.size()+1);CodeTree
res(sim
tZ2));eN.back().eN2.swap(res);eN.eB=iB.size();IP+=2;y01}
case
cJump:{CodeTree
res(sim
tZ2));eN
nM2.swap(res);eN.eB=iB[IP+1]+1;IP+=2;y01}
case
cImmed:x82
Immed[DP++]);lD
cDup:sim.Dup();lD
cNop:lD
cFCall:cF3
funcno=yJ3;assert(funcno<fpdata.mFuncPtrs.size());unsigned
c43=nN3.mFuncPtrs
cK3
mParams;sim.EatFunc(c43,lA2,funcno
xC2
case
cPCall:cF3
funcno=yJ3;assert(funcno<fpdata.iG3.size());const
FunctionParserBase<yW>&p=*nN3.iG3
cK3
mParserPtr;unsigned
c43=nN3.iG3
cK3
mParams;x4
paramlist=sim.Pop(tD2;CodeTree
tK2;tK2.GenerateFrom(*p.mData,paramlist
tY2
tK2
xC2
case
cInv:x82
1)eO2,cDiv);lD
cNeg
e12
cNeg);break;x82
0)eO2,cSub);lD
cSqr:x82
2)y51
cSqrt:x82
yI3
y51
cRSqrt:x82
yW(-0.5))y51
cCbrt:x82
yW(1)/yW(3))y51
cDeg:x82
fp_const_rad_to_deg
n51
cRad:x82
fp_const_deg_to_rad
n51
cExp:c91)goto
default_function_handling;x82
fp_const_e
t63
eO2
c63;lD
cExp2:c91)goto
default_function_handling;x82
2.0)eO2
c63;lD
cCot
e12
cTan
nZ
cCsc
e12
cSin
nZ
cSec
e12
cCos
nZ
cInt:
#ifndef __x86_64
c91){sim
cV2
cInt
xC2
#endif
x82
yI3
n03)yN
1,cFloor);lD
cLog10
e12
n13
fp_const_log10inv
n51
cLog2
e12
n13
fp_const_log2inv
n51
yQ3:sim.x0
1,n13
fp_const_log2inv
t63
yN
3,cMul);lD
cHypot:x82
2)yN
2
c63;t13
x82
2)yN
2
c63
n03);x82
yI3
y51
cSinCos:sim.Dup()yN
1,cSin);sim.x0
1,cCos);lD
cSinhCosh:sim.Dup()yN
1,cSinh);sim.x0
1,cCosh);lD
cRSub:t13
case
cSub:c91){sim.Eat(2,cSub
xC2
x82-1)yN
2,cMul)n03);lD
cRDiv:t13
case
cDiv:c91||IsIntType<yW>::cG3){sim.Eat(2,cDiv
xC2
x82-1)yN
2
c63
yN
2,cMul);lD
cAdd:case
cMul:case
cMod:case
cPow:case
cEqual:case
cLess:case
cGreater:case
t81:case
cLessOrEq:case
cGreaterOrEq:case
cAnd:case
cOr:case
cAbsAnd:case
cAbsOr:sim.Eat(2,xT1
lD
cNot:case
cNotNot:case
yL3:case
cAbsNotNot
e12
xT1
lD
cFetch:sim.Fetch(yJ3);lD
cPopNMov:cF3
stackOffs_target=yJ3;unsigned
stackOffs_source=yJ3;sim.PopNMov(stackOffs_target,stackOffs_source
xC2
#ifndef FP_DISABLE_EVAL
case
cEval:{yU3
paramcount=nN3.mVariablesAmount
yN
paramcount,xT1
break;}
#endif
default:default_function_handling:;unsigned
funcno=opcode-cAbs;assert(funcno<FUNC_AMOUNT);const
FuncDefinition&func=Functions[funcno]yN
func.c43,xT1
y93}
Become(sim
tZ2));
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Produced tree:\n"
;iG
#endif
}
}
#endif
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
#include <assert.h>
#define FP_MUL_COMBINE_EXPONENTS
l63{using
l63
FUNCTIONPARSERTYPES;using
t4
nI1
static
void
AdoptChildrenWithSameOpcode(cZ2
#ifdef DEBUG_SUBSTITUTIONS
bool
lX2=false;
#endif
t23
c92
nE==tree
nE){
#ifdef DEBUG_SUBSTITUTIONS
if(!lX2)y32"Before assimilation: "
tU2
lX2=true;}
#endif
tree.AddParamsMove
iH
l7
a).GetUniqueRef().iG1),a);}
#ifdef DEBUG_SUBSTITUTIONS
if(lX2)y32"After assimilation:   "
tU2}
#endif
}
}
t4{e01
ConstantFolding(cZ2
tree.Sort();
#ifdef DEBUG_SUBSTITUTIONS
void*yK3=0
xL1"["
<<(&yK3)<<"]Runs ConstantFolding for: "
tU2
DumpHashes
iH)xL1
std::flush;
#endif
if(false){redo:;tree.Sort();
#ifdef DEBUG_SUBSTITUTIONS
l13<<"["
<<(&yK3)<<"]Re-runs ConstantFolding: "
tU2
DumpHashes
iH);
#endif
}
if
iH
nE!=cImmed){lK3
p=nQ1
iH);if(p.min
iR2
p
yS&&p
i61==p
xN){tree.ReplaceWithImmed(p
i61);n6}
if(false){ReplaceTreeWithOne:tree.ReplaceWithImmed(yW(1));goto
do_return;ReplaceTreeWithZero:tree.ReplaceWithImmed(yW(0));goto
do_return;ReplaceTreeWithParam0:
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Before replace: "
xL1
std::hex<<'['<<yF1
hash1<<','<<yF1
hash2<<']'<<std::dec
tU2
#endif
tree
t31
0));
#ifdef DEBUG_SUBSTITUTIONS
l13<<"After replace: "
xL1
std::hex<<'['<<yF1
hash1<<','<<yF1
hash2<<']'<<std::dec
tU2
#endif
goto
redo;tL2
nE
iK2
cImmed:lD
iN2:lD
cAnd:case
cAbsAnd:cZ
bool
cA=false;t23{if(!iZ2
a)))cA=true;tM2
a),eP2
cAbsAnd)iK2
IsNever
cV
IsAlways:iL);lD
lH1
tL2
cB1
iK2
0:tZ
1:n23
eP2
cAnd?cNotNot:cAbsNotNot
t02
default:tW2
cAnd||!cA)if(ConstantFolding_AndLogic
e31
e41
cOr:case
cAbsOr:cZ
bool
cA=false;t23{if(!iZ2
a)))cA=true;tM2
a),eP2
cAbsOr)){lD2
tZ
iY2
iL);lD
lH1
tL2
cB1
iK2
0
cV
1:n23
eP2
cOr?cNotNot:cAbsNotNot
t02
default:tW2
cOr||!cA)if(ConstantFolding_OrLogic
e31
e41
cNot:case
yL3:cF3
iX
0;switch
iH
l7
0)nE
iK2
cEqual:iX
t81;lD
t81:iX
cEqual;lD
cLess:iX
cGreaterOrEq;lD
cGreater:iX
cLessOrEq;lD
cLessOrEq:iX
cGreater;lD
cGreaterOrEq:iX
cLess;lD
cNotNot:iX
cNot;lD
cNot:iX
cNotNot;lD
yL3:iX
cAbsNotNot;lD
cAbsNotNot:iX
yL3;break;default:break;}
if(opposite){n23
OPCODE(opposite));tree.SetParamsMove
iH
l7
0).GetUniqueRef().iG1)t02}
tM2
0),tree
cA1
iK2
IsAlways
cV
iY2
tZ
lH1
tW2
cNot&&GetPositivityInfo
iH
l7
0))==IsAlways)n23
yL3);yU2
nE==cIf||i42
nE==cAbsIf
lG
nO3=i42;iK1
ifp1=nO3
l7
1);iK1
ifp2=nO3
l7
2);if(ifp1
nE==cNot||ifp1
cA1{tree.x3
ifp1
nE==cNot?cNotNot:cAbsNotNot);tN2
l7
0));tO2
yM3
iP1
yN3
xF2
if(ifp2
nE==cNot||ifp2
cA1{tree.x3
tree
nE);tN2);tO2
yM3
cC
ifp2
nE==cNot?cNotNot:cAbsNotNot);yN3
l7
0)xF2
e41
cNotNot:case
cAbsNotNot:{if(iZ2
0)))e0
tM2
0),eP2
cAbsNotNot)iK2
IsNever
cV
IsAlways:tZ
lH1
tW2
cNotNot&&GetPositivityInfo
iH
l7
0))==IsAlways)n23
cAbsNotNot);yU2
nE==cIf||i42
nE==cAbsIf
lG
nO3=i42;iK1
ifp1=nO3
l7
1);iK1
ifp2=nO3
l7
2);if(ifp1
nE==cNot||ifp1
cA1{i21
0,nO3
yZ2
cJ
ifp1);yM3
iP1
yN3
xF2
if(ifp2
nE==cNot||ifp2
cA1{tree.x3
tree
nE);tN2);tO2
tree
cJ
ifp2)cU
e41
cIf:case
cAbsIf:{if(ConstantFolding_IfOperations
e31
break
cS2
cMul:{NowWeAreMulGroup:;AdoptChildrenWithSameOpcode
iH);yW
lW1=yW(1);yU3
tV1=0;bool
lX1=false
lY2
if(!xL2
xF1)y01
yW
immed=lY1
if(immed==yW(0))goto
ReplaceTreeWithZero;lW1*=immed;++tV1;}
if(tV1>1||(tV1==1&&lI3
lW1,yW(1))))lX1=true;if(lX1){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"cMul: Will add new "
iH3
lW1<<"\n"
;
#endif
t23
c92
xF1){
#ifdef DEBUG_SUBSTITUTIONS
l13<<" - For that, deleting "
iH3
lY1
l13<<"\n"
;
#endif
t33(!lI3
lW1,yW(1)))tree
cJ
yW1
lW1));tL2
cB1
iK2
0:tZ
1:e0
default:if(ConstantFolding_MulGrouping
e31
if(ConstantFolding_MulLogicItems
e31
e41
cAdd:cZ
yW
lB2=0.0;yU3
tV1=0;bool
lX1=false
lY2
if(!xL2
xF1)y01
yW
immed=lY1
lB2+=immed;++tV1;}
if(tV1>1||(tV1==1&&lB2==yW(0)))lX1=true;if(lX1){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"cAdd: Will add new "
iH3
lB2<<"\n"
xL1"In: "
tU2
#endif
t23
c92
xF1){
#ifdef DEBUG_SUBSTITUTIONS
l13<<" - For that, deleting "
iH3
lY1
l13<<"\n"
;
#endif
t33(!(lB2==yW(0.0)))tree
cJ
yW1
lB2));tL2
cB1
iK2
0
cV
1:e0
default:if(ConstantFolding_AddGrouping
e31
if(ConstantFolding_AddLogicItems
e31
e41
cMin:cZ
yU3
yC2=0;lK3
e3
lY2
while(a+1<tree
cB1&&xL2
xB
tree
l7
a+1)))iL+1);lL3
max
iR2(!e3
yS||(p
xN)<e3
xN)){e3
xN=p
xN;e3
yS=true;yC2=a;}
}
if(e3
yS)t23{lL3
min
iR2
a!=yC2&&p
i61>=e3
xN)t33
iH
yU1
1){e0
e41
cMax:cZ
yU3
yC2=0;lK3
eW
lY2
while(a+1<tree
cB1&&xL2
xB
tree
l7
a+1)))iL+1);lL3
min
iR2(!eW.min.known||p
i61>eW
i61)){eW
i61=p
i61;eW.min.known=true;yC2=a;}
}
if(eW.min.known){t23{lL3
max
iR2
a!=yC2&&(p
xN)<eW
i61){iL);}
}
}
if
iH
yU1
1){e0
e41
cEqual:case
t81:case
cLess:case
cGreater:case
cLessOrEq:case
cGreaterOrEq:if(ConstantFolding_Comparison
e31
lD
cAbs:{lK3
tM
i42);if(tN
e0
if(p0
yT{n23
cMul);tree.yG
yW(1)));goto
NowWeAreMulGroup;}
yU2
nE==cMul){iK1
p=i42;t01
n43;t01
neg_set
xG1
p.l01++a){tM
p
l7
a));if(tN{n43
i0
p
l7
a));}
if(p0
yT{neg_set
i0
p
l7
a));}
}
#ifdef DEBUG_SUBSTITUTIONS
l13<<"Abs: mul group has "
<<n43.size()<<" pos, "
<<neg_set.size()<<"neg\n"
;
#endif
if(!n43
i23||!neg_set
i23){
#ifdef DEBUG_SUBSTITUTIONS
l13<<"AbsReplace-Before: "
;DumpTree
iH)xL1"\n"
<<std::flush;DumpHashes
iH
tP2);
#endif
xA
c73;c73
cC
cMul)xG1
p.l01++a){tM
p
l7
a));if((tN||(p0
yT){}
else
c73
cJ
p
l7
a));}
c73
tZ1
xA
abs_mul;abs_mul
cC
cAbs);abs_mul
t11
c73);abs_mul
tZ1
xA
xV1
cMul);mulgroup
t11
abs_mul);t21
AddParamsMove(n43);if(!neg_set
i23){if(neg_set.size()%2)t21
yG
yW(-1)));t21
AddParamsMove(neg_set);}
tree.Become(mulgroup);
#ifdef DEBUG_SUBSTITUTIONS
l13<<"AbsReplace-After: "
;DumpTree
iH
tP2)xL1"\n"
<<std::flush;DumpHashes
iH
tP2);
#endif
goto
NowWeAreMulGroup;}
}
break;}
#define HANDLE_UNARY_CONST_FUNC(funcname) if eC3 funcname(n0));n6
case
cLog:i63(fp_log);yU2
nE==cPow
lG
pow=i42;if(GetPositivityInfo(pow
l7
0))==IsAlways){pow.iT
pow
lZ1
lR
if(GetEvennessInfo(pow
l7
1))==IsAlways){pow.iT
xA
abs;abs
cC
cAbs);abs
t11
pow
l7
0));abs
tZ1
pow
lZ1;pow.lO1
0,abs)lR}
else
yU2
nE==cAbs
lG
pow=i42
l7
0);if(pow
nE==cPow){pow.iT
xA
abs;abs
cC
cAbs);abs
t11
pow
l7
0));abs
tZ1
pow
lZ1;pow.lO1
0,abs)lR}
lD
cAcosh:i63(fp_acosh);lD
cAsinh:i63(fp_asinh);lD
cAtanh:i63(fp_atanh);lD
cAcos:i63(fp_acos);lD
cAsin:i63(fp_asin);lD
cAtan:i63(fp_atan);lD
cCosh:i63(fp_cosh);lD
cSinh:i63(fp_sinh);lD
cTanh:i63(fp_tanh);lD
cSin:i63(fp_sin);lD
cCos:i63(fp_cos);lD
cTan:i63(fp_tan);lD
cCeil:if(n5
i63(fp_ceil);lD
cTrunc:if(n5
i63(fp_trunc);lD
cFloor:if(n5
i63(fp_floor);lD
cInt:if(n5
i63(fp_int);lD
cCbrt:i63(fp_cbrt);lD
cSqrt:i63(fp_sqrt);lD
cExp:i63(fp_exp);lD
cLog2:i63(fp_log2);lD
cLog10:i63(fp_log10);lD
yQ3:lS
fp_log2(n0)*xK)c02
cMod:lS
fp_mod(n0,xK))c02
cAtan2:{lK3
tM
tree
l7
nK2
p1
nP1
1));iO3
lI3
n0,yW(0))){if(p1
yS&&(p1
xN)<yW(0
nJ
fp_const_pi
t63;n6
if(p1.min
iR2
p1
i61>=yW(0.0
nJ
yW(0));n6}
if
iH
tS2
lI3
xK,yW(0))){if(p0
yS&&(p0
xN)<yW(0
nJ-fp_const_pihalf
t63;n6
if(lC2
p0
i61>yW(0
nJ
fp_const_pihalf
t63;n6}
lS
fp_atan2(n0,xK));n6
if((p1.min
iR2
p1
i61>yW(0))||(p1
yS&&(p1
xN)<fp_const_negativezero
t63
lG
yD2;yD2
cC
cPow);yD2.AddParamMove
iH
l7
1));yD2.yG
yW(-1)));yD2
tZ1
xA
yE2;yE2
x03
yE2.AddParamMove
iH
l7
0));yE2
t11
yD2);yE2
tZ1
n23
cAtan);tree.lO1
0,yE2)eZ1
1);e41
cPow:{if(ConstantFolding_PowOperations
e31
break
cS2
cDiv:iO3
e02
xF1&&xK!=yW(0.0
nJ
n0/xK)c02
cInv:iO3
n0!=yW(0.0
nJ
yW(1)/n0)c02
cSub:lS
n0-xK)c02
cNeg:if
eC3-n0)c02
cRad:if
eC3
RadiansToDegrees(n0))c02
cDeg:if
eC3
DegreesToRadians(n0))c02
cSqr:if
eC3
n0*n0)c02
cExp2:i63(fp_exp2);lD
cRSqrt:if
eC3
yW(1)/fp_sqrt(n0))c02
cCot:if(eD3
fp_tan(n0);xJ
cSec:if(eD3
fp_cos(n0);xJ
cCsc:if(eD3
fp_sin(n0);xJ
cHypot:lS
fp_hypot(n0,xK))c02
cRDiv:case
cRSub:case
cDup:case
cFetch:case
cPopNMov:case
cSinCos:case
cSinhCosh:case
cNop:case
cJump:lD
cPCall:case
cFCall:case
cEval:break;}
do_return:;
#ifdef DEBUG_SUBSTITUTIONS
l13<<"["
<<(&yK3)<<"]Done ConstantFolding, result: "
tU2
DumpHashes
iH);
#endif
}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
t4{e01
lK3::set_abs(nN
bool
has_negative=!min.known||min.val<yW();bool
has_positive=!n53||t43>yW();bool
crosses_axis=has_negative&&has_positive;c11<yW>newmax;if(min
iR2
n53)newmax.set(fp_max(tA1,tB1);if(crosses_axis
tI3
yW());tC3(min
iR2
n53
tI3
fp_min(tA1,tB1);nC2
min.known
tI3
tA1);else
min.set(tB1;}
max=newmax;}
e01
lK3::set_neg(){std::swap(min,max);min.val=-min.val;t43=-t43;}
x61
IsLogicalTrueValue
n92
lK3&p
xA3{if(nD
IsIntType<yW>::cG3){if(p.min
iR2
p
i61>=yW(1))tW1
if(!abs&&p
iS2<=yW(-1))tW1}
tC3(p.min
iR2
p
i61>=yI3
tW1
if(!abs&&p
iS2<=yW(-0.5))tW1
tD3
e03
x61
IsLogicalFalseValue
n92
lK3&p
xA3{if(nD
IsIntType<yW>::cG3){if(abs)tE3
iS2<yW(1);else
tE3.min
iR2
p
yS&&p
i61>yW(-1)&&p
xN<yW(1);}
tC3(abs)tE3
iS2<yW(0.5);else
tE3.min
iR2
p
yS&&p
i61>yW(-0.5)&&p
xN<yW(0.5);}
}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;using
t4;t4{eQ1
lK3
nQ1
iS1
#ifdef DEBUG_SUBSTITUTIONS_extra_verbose
{using
l63
FUNCTIONPARSERTYPES;lK3
tmp=CalculateResultBoundaries_do
iH)xL1"Estimated boundaries: "
;if(tmp.min.known)l13<<tmp
i61;else
l13<<"-inf"
xL1" .. "
;if(tmp
yS)l13<<tmp
xN;else
l13<<"+inf"
xL1": "
;DumpTree
iH)xL1
std::endl
xD1
tmp;}
eQ1
lK3
xA::CalculateResultBoundaries_do
iS1
#endif
{tW
y61(-t73,fp_const_pihalf
t63;tW
pi_limits(-fp_const_pi<yW>(),fp_const_pi
t63;tW
abs_pi_limits(yW(0),fp_const_pi
t63;tW
plusminus1_limits(yW(-t93
using
l63
std;switch
iH
nE
iK2
cImmed:nO
tree
nJ1,tree
nJ1);case
cAnd:case
cAbsAnd:case
cOr:case
cAbsOr:case
cNot:case
yL3:case
cNotNot:case
cAbsNotNot:case
cEqual:case
t81:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:{nO
yW(0),yW(1))cS2
cAbs:lC
m.set_abs(tG
cLog:lC
n63
fp_log);m
cD2
fp_log
tG
cLog2:lC
n63
fp_log2);m
cD2
fp_log2
tG
cLog10:lC
n63
fp_log10);m
cD2
fp_log10
tG
cAcosh:lC
cG
eR1
cGreaterOrEq
iX2
fp_acosh
tU3
cGreaterOrEq
iX2
fp_acosh
tG
cAsinh:lC
cG
set(fp_asinh
x23.set(fp_asinh
tG
cAtanh:lC
cG
n4-1),fp_atanh
tU3
cLess
iX2
fp_atanh
tG
cAcos:lC
nO(m
yS&&(m
xN)<yW(1))?fp_acos(m
xN):yW(0),(n73&&cA2)>=yW(-1))?fp_acos
cA2):fp_const_pi
t63
cS2
cAsin:lC
cG
n4-1),fp_asin,y61
i61
tU3
cLess
iX2
fp_asin,y61
xN
tG
cAtan:lC
cG
set(fp_atan,y61
i61
x23.set(fp_atan,y61
xN
tG
cAtan2:{lK3
tM
tree
l7
nK2
p1
nP1
1));iO3
lI3
n0,yW(0)))yZ
abs_pi_limits;}
if
iH
tS2
lI3
xK,yW(0)))yZ
y61;tD3
pi_limits
cS2
cSin:lC
bool
nM1=!n73||!m
yS||(m
xN-cG
val)>=(cD
nM1)nN1
yW
min=fp_mod
cA2,cD
min
t53
min
cF
yW
max=fp_mod(m
xN,cD
max
t53
max
cF
if(max<min)max
cF
bool
covers_plus1=(min<=t73&&max>=fp_const_pihalf
t63;bool
n01=(min<=yW(tX&&max>=yW(tX
nO1&&n01)nN1
if(n01)nO
yW(-1),eQ2
nO1)nO
yF2
yW(1));nO
yF2
eQ2)cS2
cCos:lC
if(n73)cG
val+=t73;if(m
yS)m
xN+=t73;bool
nM1=!n73||!m
yS||(m
xN-cG
val)>=(cD
nM1)nN1
yW
min=fp_mod
cA2,cD
min
t53
min
cF
yW
max=fp_mod(m
xN,cD
max
t53
max
cF
if(max<min)max
cF
bool
covers_plus1=(min<=t73&&max>=fp_const_pihalf
t63;bool
n01=(min<=yW(tX&&max>=yW(tX
nO1&&n01)nN1
if(n01)nO
yW(-1),eQ2
nO1)nO
yF2
yW(1));nO
yF2
eQ2)cS2
cTan:{nO)cS2
cCeil:lC
m
eK
cFloor:lC
cG
yG2
tG
cTrunc:lC
cG
yG2);m
eK
cInt:lC
cG
yG2);m
eK
cSinh:lC
cG
set(fp_sinh
x23.set(fp_sinh
tG
cTanh:lC
cG
set(fp_tanh,plusminus1_limits.min
x23.set(fp_tanh,plusminus1_limits.max
tG
cCosh:lC
if(n73){if(m
yS){if
cA2
yJ2&&m
xN
yJ2){cG
val
cB}
nC2
cA2)<i13
m
xN
yJ2){yW
tmp
cB
if(tmp>m
xN)m
xN=tmp;cG
val=yW(1);}
else{cG
val
cB
std::swap
cA2,m
xN);}
}
tC3
cA2
yJ2){m
n32;cG
val=fp_cosh
cA2);}
else{m
n32;cG
val=yW(1);}
}
}
else{n73=true;cG
val=yW(1);if(m
yS){cG
val=fp_cosh(m
xN);m
n32;}
else
m
n32;tD3
m
cS2
cIf:case
cAbsIf:{lK3
res1
nP1
1));lK3
res2
nP1
2));if(!res2.min.known)res1.min.known=false;nC2
res1.min
iR2(res2
i61)<res1
i61)res1
i61=res2
i61;if(!res2
yS)res1
n32;nC2
res1
yS&&(res2
xN)>res1
xN)res1
xN=res2
xN
xD1
res1
cS2
cMin:{bool
i7=false;bool
i8=false;lK3
cG3;x5
m
e8!n73)i7
i9.min.known||cA2)<cG3
i61
cC3=cG
val;if(!m
yS)i8
i9
yS||(m
xN)<cG3
xN)cG3
xN=m
xN;}
if(i7)cG3
cB2
i8)cG3
n32
xD1
cG3
cS2
cMax:{bool
i7=false;bool
i8=false;lK3
cG3;x5
m
e8!n73)i7
i9.min.known||cG
val>cG3
i61
cC3=cG
val;if(!m
yS)i8
i9
yS||m
xN>cG3
xN)cG3
xN=m
xN;}
if(i7)cG3
cB2
i8)cG3
n32
xD1
cG3
cS2
cAdd:{lK3
cA3
yW(0),yW(0));x5
item
e8
item.min.known
cC3+=item
i61;else
cG3
cB2
item
yS)cG3
xN+=item
xN;else
cG3
n32;if(!cB3
iR2!cD3)break;}
if
yZ3.min
iR2
cD3&&cG3
i61>cG3
xN)std::swap
yZ3
i61,cG3
xN)xD1
cG3
cS2
cMul:{cN2
Value{enum
n83{tQ2,iC1,tR2}
;n83
eC;yW
value;Value(n83
t):eC(t),value(0){}
Value(yW
v):eC(tQ2),value(v){}
bool
cE2
yJ1
eC==iC1||cJ3
value<yW(0)cM3
cS1*=n92
Value&rhs){if
cJ3
rhs.eC==tQ2)value*=rhs.value;else
eC=(cE2)!=rhs.cE2)?iC1:tR2);}
iC2<n92
Value&rhs
yJ1(eC==iC1&&rhs.eC!=iC1)||cJ3(rhs.eC==tR2||(rhs.eC==tQ2&&value<rhs.value)));}
}
;cN2
y71{Value
yH2,yI2;y71():yH2(Value::tR2),yI2(Value::iC1){}
void
multiply(Value
yO3,const
Value&value2){yO3*=value2;if(yO3<yH2)yH2=yO3;if(yI2<yO3)yI2=yO3;}
}
;lK3
cA3
yW(t93
x5
item
e8!item.min
iR2!item
yS)nO);Value
n93=cB3.known?Value
yZ3
i61):i51
iC1);Value
nA3=cD3?Value
yZ3
xN):i51
tR2);Value
nB3=item.min.known?Value(item
i61):i51
iC1);Value
nC3=item
yS?Value(item
xN):i51
tR2);y71
range;range.multiply(n93,nB3
e71
n93,nC3
e71
nA3,nB3
e71
nA3,nC3);if(range.yH2.eC==Value::tQ2
cC3=range.yH2.value;else
cG3
cB2
range.yI2.eC==Value::tQ2)cG3
xN=range.yI2.value;else
cG3
n32;if(!cB3
iR2!cD3)break;}
if
yZ3.min
iR2
cD3&&cG3
i61>cG3
xN)std::swap
yZ3
i61,cG3
xN)xD1
cG3
cS2
cMod:{lK3
x
nP1
nK2
y
nP1
1));if(y
yS){if(y
xN
yJ2){if(!x.min.known||(x
i61)t53
nO-y
xN,y
xN);else
nO
yW(0),y
xN);}
tC3(!x
yS||(x
xN)yJ2)nO
y
xN,-y
xN);else
nO
y
xN,fp_const_negativezero
t63;}
}
else
nO)cS2
cPow:{if
iH
tS2
xK==yW(0)){nO
yW(t93}
iO3
n0==yW(0)){nO
yW(0),yW(0));}
iO3
lI3
n0
nI3
nO
yW(t93}
if
iH
tS2
xK>i13
GetEvennessInfo
iH
l7
1))==IsAlways){yW
eB2
xK;lK3
tmp
nP1
nK2
cG3;cB3.known=true;cG3
i61=0;if(tmp.min
iR2
tmp
i61
yJ2
cC3
xV3
tmp
i61,nW2;nC2
tmp
yS&&tmp
xN<=yW(0)cC3
xV3
tmp
xN,nW2;cG3
n32;if(tmp.min
iR2
tmp
yS){cD3=true;cG3
xN=fp_max(fp_abs(tmp
i61),fp_abs(tmp
xN));cG3
xN
xV3
cG3
xN,nW2;tD3
cG3;}
lK3
tM
tree
l7
nK2
p1
nP1
1
iM2
p0_positivity=(lC2(p0
i61)yJ2)?IsAlways:(p0
yS&&(p0
xN)tV3
iY2
Unknown);TriTruthValue
cF2=GetEvennessInfo
iH
l7
1
iM2
eX=Unknown;switch(p0_positivity){lD2
eX=IsAlways;lD
iY2{eX=cF2;break;}
default:switch(cF2){lD2
eX=IsAlways;lD
iY2
lD
Unknown:{if
iH
tS2!t52
xK)&&xK
yJ2){eX=IsAlways;}
y93}
switch(eX){lD2{yW
min=yW(0);if(lC2
p1.min.known){min
xV3
p0
i61,p1
i61);if(p0
i61<i13(!p1
yS||p1
xN
yJ2)&&min
yJ2)min=yW(0);}
if(lC2
p0
i61
yJ2&&p0
yS&&p1
yS){yW
max
xV3
p0
xN,p1
xN);if(min>max)std::swap(min,max);nO
min,max);}
nO
min,false)cS2
iY2{nO
false,fp_const_negativezero
t63;}
default:{break;}
e41
cNeg:lC
m.set_neg(tG
cSub:n7
cNeg
lG2
1));tmp
cC
cAdd
tV2
tmp
t11
tmp2);nZ1
cInv:{cG2-1)));nZ1
cDiv:n7
cInv
lG2
1));tmp
cC
nW1
n12
tmp2);nZ1
cRad:{eP1
nW1
yG
fp_const_rad_to_deg
t63);nZ1
cDeg:{eP1
nW1
yG
fp_const_deg_to_rad
t63);nZ1
cSqr:{cG2
2)));nZ1
cExp:{eP1
cPow);tmp.yG
fp_const_e
t63
tV2
nZ1
cExp2:{eP1
cPow);tmp.yG
yW(2))tV2
nZ1
cCbrt:lC
cG
set(fp_cbrt
x23.set(fp_cbrt
tG
cSqrt:lC
if(n73)cG
val=cA2)tV3
0:fp_sqrt
cA2);if(m
yS)m
xN=(m
xN)tV3
0:fp_sqrt(m
xN
tG
cRSqrt:{cG2-0.5)));nZ1
cHypot:{xA
xsqr,ysqr,add,sqrt;xsqr.nL
0));xsqr.yG
yW(2)));ysqr.nL
1));ysqr.yG
yW(2)));xsqr
cC
cPow);ysqr
cC
cPow);add
t11
xsqr);add
t11
ysqr);add
cC
cAdd);sqrt
t11
add);sqrt
cC
cSqrt)xD1
nQ1(sqrt)cS2
yQ3:n7
cLog2
lG2
0));tmp
x03
tmp
t11
tmp2);tmp.nL
1));nZ1
cCot:n7
cTan
lG2
yD
lI
cSec:n7
cCos
lG2
yD
lI
cCsc:n7
cSin
lG2
yD
nQ1(tmp);}
lD
cRDiv:case
cRSub:case
cDup:case
cFetch:case
cPopNMov:case
cSinCos:case
cSinhCosh:case
cNop:case
cJump:case
iN2:lD
cPCall:lD
cFCall:lD
cEval:break;}
nO);}
eQ1
TriTruthValue
GetIntegerInfo
iS1{switch
iH
nE
iK2
cImmed:return
isInteger
iH
nJ1)?IsAlways:IsNever;case
cFloor:case
cCeil:case
cTrunc:case
cInt:return
IsAlways;case
cAnd:case
cOr:case
cNot:case
cNotNot:case
cEqual:case
t81:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:return
IsAlways;case
cIf:{TriTruthValue
a=GetIntegerInfo
iH
l7
1
iM2
b=GetIntegerInfo
iH
l7
2));if(a==b)return
a
xD1
Unknown
cS2
cAdd:case
cMul:{t23
if(GetIntegerInfo
iH
l7
a))!=IsAlways)return
Unknown
xD1
IsAlways;}
default:break;tD3
Unknown;}
x61
IsLogicalValue
iS1{switch
iH
nE
iK2
cImmed:return
fp_equal
iH
nJ1,yW(0))||fp_equal
iH
nJ1,yW(1));case
cAnd:case
cOr:case
cNot:case
cNotNot:case
cAbsAnd:case
cAbsOr:case
yL3:case
cAbsNotNot:case
cEqual:case
t81:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:nW
cMul:{t23
if(!iZ2
a))y5
tW1}
case
cIf:case
cAbsIf:yZ
iZ2
1))&&iZ2
2));}
default:break;tD3
e03}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;
#if defined(__x86_64) || !defined(FP_SUPPORT_CBRT)
# define CBRT_IS_SLOW
#endif
#if defined(DEBUG_POWI) || defined(DEBUG_SUBSTITUTIONS)
#include <cstdio>
#endif
l63
x11{extern
const
unsigned
char
powi_table[256];}
l63{using
t4
nI1
bool
IsOptimizableUsingPowi(long
immed,long
penalty=0){x11::n81
synth;synth.PushVar(iN2);yU3
bytecodesize_backup
eZ3
GetByteCodeSize();x11::nL1
immed,x11::eX1<yW>::MulSequence,cV1;yU3
bytecode_grow_amount
eZ3
GetByteCodeSize()-bytecodesize_backup
xD1
bytecode_grow_amount<yU3(MAX_POWI_BYTECODE_LENGTH-penalty);}
e01
ChangeIntoRootChain(xA&tree,bool
l03,long
i02,long
i12){while(i12>0
nN2
cCbrt);tW3
tmp
tZ1
tree.i22--i12;}
while(i02>0
nN2
cSqrt);if(l03){tmp
cC
cRSqrt);l03=e03
tW3
tmp
tZ1
tree.i22--i02;}
if(l03
nN2
cInv);tW3
tree.i22}
}
eQ1
cN2
RootPowerTable{static
const
yW
RootPowers[(1+4)*(1+3)];}
nI1
const
yW
eY(1+4)*(1+3)]={yW(1)lP
tX3
tX3
2*tX3
nE2)lP
3*2)lP
3*2*2)lP
3*nE2*2*nE2*3
tX2
2
tX2
2*2
tX2
nE2*3*2*nE2*3*3
tX2
3*2
tX2
3*2*2
tX2
3*nE2*3*3*2*2*2*2)}
;cN2
PowiResolver{static
const
unsigned
MaxSep=4;static
nG3
tY3=5;typedef
int
yP3;typedef
long
nP3;typedef
long
iF;cN2
yK2{yK2():n_int_sqrt(0),n_int_cbrt(0),sep_list(),lI1(0){}
int
n_int_sqrt;int
n_int_cbrt;int
sep_list[MaxSep];iF
lI1;}
nI1
static
yK2
CreatePowiResult(yW
nW2{yK2
cG3;yP3
eZ=FindIntegerFactor(nW2;if(eZ==0){
#ifdef DEBUG_POWI
i32"no factor found for %Lg\n"
,yR1
nW2;
#endif
return
cG3;}
cG3.lI1=xU1
nR3,eZ);nP3
eR2=EvaluateFactorCost(eZ,0,0,0)+cL
cG3.lI1);int
tZ3=0;int
i03=0;int
nD3=0;
#ifdef DEBUG_POWI
i32"orig = %Lg\n"
,yR1
nW2;i32"plain factor = "
iI3"%ld\n"
,(int)eZ,(long)eR2);
#endif
for
iK
n_s=0;n_s<MaxSep;++n_s){int
xC=0;nP3
y81=eR2;yP3
yG1=eZ;for(int
s=1;s<tY3*4;++s){
#ifdef CBRT_IS_SLOW
if(s>=tY3)break;
#endif
int
n_sqrt=s%tY3;int
n_cbrt=s/tY3;if(n_sqrt+n_cbrt>4)y01
yW
l71=nR3;l71-=eY
s];tC1=FindIntegerFactor(l71);if
nL2!=0){iF
xP=xU1
l71,yS2);nP3
cost=EvaluateFactorCost
nL2,tZ3+n_sqrt,i03+n_cbrt,nD3+1)+cL
xP);
#ifdef DEBUG_POWI
i32"Candidate sep %u (%d*sqrt %d*cbrt)factor = "
iI3"%ld (for %Lg to %ld)\n"
,s,n_sqrt,n_cbrt,yS2,(long)cost,yR1
l71,(long)xP);
#endif
if(cost<y81){xC=s;yG1=yS2;y81=cost;}
}
}
if(!xC)break;
#ifdef DEBUG_POWI
i32"CHOSEN sep %u (%d*sqrt %d*cbrt)factor = "
iI3"%ld, exponent %Lg->%Lg\n"
,xC,xC%tY3,xC/tY3,yG1,y81,yR1(nW2,yR1(nR3-eY
xC]));
#endif
cG3
e91
n_s]=xC;nR3-=eY
xC];tZ3+=xC%tY3;i03+=xC/tY3;eR2=y81;eZ=yG1;nD3+=1;}
cG3.lI1=xU1
nR3,eZ);
#ifdef DEBUG_POWI
i32"resulting exponent is %ld (from exponent=%Lg, best_factor=%Lg)\n"
,cG3.lI1,yR1
nR3,yR1
eZ);
#endif
while(eZ%2==0){++cG3
cJ2;eZ/=2;}
while(eZ%3==0){++cG3.n_int_cbrt;eZ/=3;tD3
cG3;}
private:static
nP3
cL
iF
xP){static
std::map
cH2
i3;if(xP<0){nP3
cost=22
xD1
cost+cL-xP);}
std::map
cH2::xN3
i=i3.xI2
xP);if(i!=i3
yQ1
xP)return
i->second;std::pair
cH2
cA3
xP,0.0);nP3&cost=cG3.second;while(xP>1){int
yS2=0;if(xP<256){yS2=x11::powi_table[xP];if
nL2&128)yS2&=127;else
yS2=0;if
nL2&64)yS2=-nL2&63)-1;}
if
nL2){cost+=cL
yS2);xP/=yS2;y01}
if(!(xP&1)){xP/=2;cost+=6;}
else{cost+=7;xP-=1;}
}
i3.xK3,cG3)xD1
cost;}
yY1
iF
xU1
lE2,tC1)yZ
makeLongInteger(value*yW
nL2));}
yY1
bool
y91
lE2,tC1){yW
v=value*yW
nL2)xD1
isLongInteger(v);}
yY1
yP3
FindIntegerFactor(lE2){tC1=(2*2*2*2);
#ifdef CBRT_IS_SLOW
#else
yS2*=(3*3*3);
#endif
yP3
nE3
0;if(y91
value,yS2)){nE3
yS2;while(nL2%2)==0&&y91
value,yS2/2))nE3
yS2/=2;while(nL2%3)==0&&y91
value,yS2/3))nE3
yS2/=3;}
#ifdef CBRT_IS_SLOW
if
yZ3==0){if(y91
value,3
lN3
3;}
#endif
return
cG3;}
static
int
EvaluateFactorCost(int
yS2,int
s,int
c,int
nmuls){nG3
nH3=6;
#ifdef CBRT_IS_SLOW
nG3
eS2=25;
#else
nG3
eS2=8;
#endif
int
nE3
s*nH3+c*eS2;while
nL2%2==0){yS2/=2;cG3+=nH3;}
while
nL2%3==0){yS2/=3;cG3+=eS2;}
cG3+=nmuls
xD1
cG3;}
}
;}
t4{x61
xA::RecreateInversionsAndNegations(bool
prefer_base2){bool
changed=false
xG1
l01++a)if(lG1.RecreateInversionsAndNegations(prefer_base2))nP2
if(changed){exit_changed:Mark_Incompletely_Hashed()xA1
switch(iA1{case
cMul:{t01
lH2;xA
lI2,cC1;if(true){bool
n11=false;yW
nO2=0;xI1
nS
yL2
0)cI2
tC
1)xF1){n11=true;nO2=tC
1)nJ1;y93
if(n11){yW
immeds=1.0;xI1
nS
xF1){immeds*=powgroup
nJ1;yA1}
xI1-->0;lG&powgroup=lG1;if(powgroup
yL2
0)cI2
tC
1)xF1
lG&log2=tC
0);log2.iT
log2
cC
yQ3);log2.yG
fp_pow(immeds,yW(1)/nO2)));log2
tZ1
y93}
}
xI1
nS
yL2
1)xF1){iK1
exp_param=tC
1);yW
eB2
exp_param
nJ1;if(cJ1,yW(-1))){iT
lH2
i0
lG1
l7
0));yA1
nC2
nR3<i13
t52
nW2
lG
iA;iA
cC
cPow);iA
cJ
tC
0));iA.yG-nW2);iA
tZ1
lH2
i0
iA);iT
yA1}
nC2
powgroup
cI2!lI2.lZ2){lI2=tC
0);iT
yA1
nC2
powgroup
nE==yQ3&&!cC1.lZ2){cC1=powgroup;iT
yA1}
if(!lH2
i23){nP2
xA
iB1;iB1
x03
iB1
y92
lH2);iB1
tZ1
xA
xV1
cMul);t21
SetParamsMove(t0
if(mulgroup
xF1&&lI3
mulgroup
nJ1
nI3
lJ2
cInv
tJ3;}
tC3(t21
GetDepth()>=iB1
xS2){lJ2
cDiv)tK3
tJ3;}
else{lJ2
cRDiv
tJ3
tK3);}
}
}
if(lI2.lZ2
lG
xV1
iA1;t21
SetParamsMove(t0
while(t21
RecreateInversionsAndNegations(prefer_base2))t21
FixIncompleteHashes();lJ2
yQ3)e9
lI2)tK3);nP2}
if(cC1.lZ2
lG
xV1
cMul);mulgroup
t11
cC1
l7
1));t21
AddParamsMove(t0
while(t21
RecreateInversionsAndNegations(prefer_base2))t21
FixIncompleteHashes();DelParams();lJ2
yQ3)e9
cC1
l7
0))tK3);nP2
e41
cAdd:{t01
i52;xI1-->0;)if(yR3
cMul){lK2
xW1:;xA&mulgroup
nR2
eM
for(yU3
b=t21
l01
b-->0;){if(mulgroup
l7
b)xS3
yS2=mulgroup
l7
b)nJ1;iD1
yS2
xU
xW1;}
t21
iT
t21
iY1
b);t1=!t1;}
else
iD1
yS2,yW(-2)))eD
xW1;}
t21
iT
t21
iY1
b);t21
yG
yW(2)))i62}
if(t1){t21
t6
mulgroup);yA1}
nC2
yR3
cDiv&&!IsIntType<yW>::cG3){lK2
xX1:;xA&iB1
nR2
eM
if(iB1
l7
0)xF1){iD1
iB1
l7
0)nJ1
xU
xX1;}
iB1.iT
iB1.iY1
0);iB1
cC
cInv)i62
if(t1)eD
xX1;}
iB1.t6
iB1);yA1}
nC2
yR3
cRDiv&&!IsIntType<yW>::cG3){lK2
nX1:;xA&iB1
nR2
eM
if(iB1
l7
1)xF1){iD1
iB1
l7
1)nJ1
xU
nX1;}
iB1.iT
iB1.iY1
1);iB1
cC
cInv)i62
if(t1)eD
nX1;}
iB1.t6
iB1);yA1}
if(!i52
i23){
#ifdef DEBUG_SUBSTITUTIONS
i32"Will make a Sub conversion in:\n"
);fflush(stdout);iG
#endif
xA
yM2;yM2
cC
cAdd);yM2
y92
i52);yM2
tZ1
xA
cD1;cD1
cC
cAdd);cD1
y92
iG1));cD1
tZ1
if(cD1
xF1&&lI3
cD1
nJ1,yW(0))){lJ2
cNeg
e81);}
tC3(cD1
xS2==1){lJ2
cRSub
e81)e9
cD1);}
nC2
yM2
nE==cAdd){lJ2
cSub)e9
cD1
e81
l7
0))xR3
1;a<yM2.l01++a
lG
eT2;eT2
cC
cSub);eT2
y92
iG1));eT2.y82)e9
eT2
e81
l7
a));}
}
else{lJ2
cSub)e9
cD1
e81);}
}
#ifdef DEBUG_SUBSTITUTIONS
i32"After Sub conversion:\n"
);fflush(stdout);iG
#endif
e41
cPow:{iK1
p0
nR2(0);iK1
p1
nR2(1);if(p1
xF1){if(p1
nJ1!=i13!t52
p1
nJ1)){eH
yK2
r=eH
CreatePowiResult(fp_abs
tQ3);if(r.lI1!=0){bool
iE1=false;if(p1
nJ1<i13
r
e91
0]==0&&r
cJ2>0){iE1=true;}
#ifdef DEBUG_POWI
i32"Will resolve powi %Lg as powi(chain(%d,%d),%ld)"
,yR1
fp_abs
tQ3,r
cJ2,r.n_int_cbrt,r.lI1);for
iK
n=0;n<eH
MaxSep;++n){if(r
e91
n]==0)break;int
n_sqrt=r
e91
n]%eH
tY3;int
n_cbrt=r
e91
n]/eH
tY3;i32"*chain(%d,%d)"
,n_sqrt,n_cbrt);}
i32"\n"
);
#endif
xA
cK2
nR2(0);xA
yN2=cK2;yN2.iT
ChangeIntoRootChain(yN2,iE1,r
cJ2,r.n_int_cbrt);yN2
tZ1
xA
pow;if(r.lI1!=1){pow
cC
cPow);pow
t11
yN2);pow.yG
yW(r.lI1)));}
else
pow.swap(yN2);xA
mul;mul
x03
mul
t11
pow);for
iK
n=0;n<eH
MaxSep;++n){if(r
e91
n]==0)break;int
n_sqrt=r
e91
n]%eH
tY3;int
n_cbrt=r
e91
n]/eH
tY3;xA
eU2=cK2;eU2.iT
ChangeIntoRootChain(eU2,false,n_sqrt,n_cbrt);eU2
tZ1
mul
t11
eU2);}
if(p1
nJ1<i13!iE1){mul
tZ1
lJ2
cInv);lO1
0,mul);iY1
1);}
else{lJ2
cMul);SetParamsMove(mul.iG1));}
#ifdef DEBUG_POWI
iG
#endif
nP2
y93}
if(GetOpcode()==cPow&&(!p1
xF1||!isLongInteger
tQ3||!IsOptimizableUsingPowi<yW>(makeLongInteger
tQ3))){if(p0
xF1&&p0
nJ1>yW(0.0)){if(prefer_base2){yW
yO2=fp_log2(p0
nJ1);iD1
yO2
nI3
iY1
0);}
else{lX
yW1
yO2));nR3
cJ
p1
cX1
tF}
lJ2
cExp2);nP2}
else{yW
yO2=fp_log(p0
nJ1);iD1
yO2
nI3
iY1
0);}
else{lX
yW1
yO2));nR3
cJ
p1
cX1
tF}
lJ2
cExp);nP2}
}
nC2
GetPositivityInfo(p0)==IsAlways){if(prefer_base2
lG
log;log
cC
cLog2);log
cJ
p0);log
tZ1
lX
p1);nR3
t11
log
cX1;lJ2
cExp2)tF
nP2}
else{xA
log;log
cC
cLog);log
cJ
p0);log
tZ1
lX
p1);nR3
t11
log
cX1;lJ2
cExp)tF
nP2}
}
e41
cDiv:{if(GetParam(0)xF1&&lI3
GetParam(0)nJ1
nI3
lJ2
cInv);iY1
0);}
break;}
default:break;}
if(changed)goto
exit_changed
xD1
changed;}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
l63
FUNCTIONPARSERTYPES;l63{using
t4;class
nK3{yU3
n21;yU3
eI;yU3
eJ;yU3
iY;yU3
t2;yU3
t3;yU3
lL1;cI3
nK3():n21(0),eI(0),eJ(0),iY(0),t2(0),t3(0),lL1(0){}
void
tR3
OPCODE
op){n21+=1;nJ3
cCos)++eI;nJ3
cSin)++eJ;nJ3
cSec)++eI;nJ3
cCsc)++eJ;nJ3
cTan)++iY;nJ3
cCot)++iY;nJ3
cSinh)++t3;nJ3
cCosh)++t2;nJ3
cTanh)++lL1;}
yU3
GetCSEscore
tL3
yU3
nE3
n21
xD1
cG3;}
int
NeedsSinCos
tL3
bool
xY1=(n21==(eI+eJ+iY));if((iY&&(eJ||eI))||(eJ&&eI)){if(xY1)return
1
xD1
2;tD3
0;}
int
NeedsSinhCosh
tL3
bool
xY1=(n21==(t2+t3+lL1));if((lL1&&(t3||t2))||(t3&&t2)){if(xY1)return
1
xD1
2;tD3
0;}
yU3
MinimumDepth
tL3
yU3
n_sincos=std::min(eI,eJ);yU3
n_sinhcosh=std::min(t2,t3);if(n_sincos==0&&n_sinhcosh==0)return
2
xD1
1;}
}
nI1
class
TreeCountType:public
std::multimap<fphash_t,std::pair<nK3,xA> >{}
x43
FindTreeCounts(cR1&lL2,x53,OPCODE
nS2,bool
skip_root=false){yV
i=lL2.lower_bound
iH.GetHash());if(!skip_root){bool
found=false;for(;i!=lL2
yQ1
tree.GetHash();++i){if
iH
xB
i
eA2
second)){i
eA2
first.tR3
nS2);found=true;y93
if(!found){nK3
count;count.tR3
nS2);lL2.xK3,std::make_pair
iH.GetHash(),std::make_pair
yX3,tree)));}
}
e22
xW++a)FindTreeCounts(lL2,xL2,tree
nE);}
cN2
c3{bool
BalanceGood;bool
FoundChild;}
nI1
c3
iZ
iK1
root,iK1
child){if(root
xB
child)){c3
nE3{true,true}
xD1
cG3;}
c3
nE3{true,false}
;if(root
nE==cIf||root
nE==cY3{c3
cond
nF3
0
i72
c3
xY
nF3
1
i72
c3
y1
nF3
2
i72
if
lY3||xY
c4||y1
c4){cG3
c4=true;}
cG3
eE=((xY
c4==y1
c4)||lY3
cL2&&(cond
eE||(xY
c4&&y1
c4))&&(xY
eE||lY3
cL2&&(y1
eE||lY3
cL2;}
else{bool
tD1=false;bool
n31=false;for(yU3
b=root
cB1,a=0;a<b;++a){c3
tmp
nF3
a
i72
if(tmp
c4)cG3
c4=true;if(tmp
eE==false)tD1=true;nC2
tmp
c4)n31=true;}
if(tD1&&!n31)cG3
eE=e03
return
cG3;}
x61
l23
iK1
i53
x53,const
x11::n81&synth,const
cR1&lL2){for(yU3
b=tree
cB1,a=0;a<b;++a){iK1
leaf=xL2;yV
synth_it;e72
cR1::const_iterator
i=lL2.xM3
i!=lL2
tF3;++i){if(i->first!=leaf.GetHash())y01
const
nK3&occ
n02
first;yU3
score=occ.GetCSEscore();iK1
candidate
n02
second;if(lM2(candidate))y01
if(leaf
xS2<occ.MinimumDepth())y01
if(score<2)y01
if(iZ
i53
leaf)eE==false)continue
xA1
if(l23
i53
leaf,synth,lL2))tW1
tD3
e03
x61
tE1
iK1
xL3,iK1
expr){yH1
xL3
l7
a)xB
expr))tW1
yH1
tE1
xL3
l7
a),expr
lN3
true
xD1
e03
x61
GoodMomentForCSE(iK1
xL3,iK1
expr){if(xL3
nE==cIf)tW1
yH1
xL3
l7
a)xB
expr))tW1
yU3
i82=0;yH1
tE1
xL3
l7
a),expr))++i82
xD1
i82!=1;}
}
t4{eQ1
yU3
xA::SynthCommonSubExpressions(x11::n81&cV1
const{if(GetParamCount()==0)return
0;yU3
stacktop_before
eZ3
GetStackTop();cR1
lL2;FindTreeCounts(lL2,*this,GetOpcode(),true);for(;;){yU3
yP2=0;
#ifdef DEBUG_SUBSTITUTIONS_CSE
l13<<"Finding a CSE candidate, root is:"
<<l53
DumpHashes(*this);
#endif
yV
cs_it(lL2
tF3);for(yV
j=lL2.xM3
j!=lL2
tF3;){yV
i(j++);const
nK3&occ
n02
first;yU3
score=occ.GetCSEscore();x53
n02
second;
#ifdef DEBUG_SUBSTITUTIONS_CSE
l13<<"Score "
<<score<<":\n"
<<std::flush;DumpTreeWithIndent
iH);
#endif
if(lM2
iH))xS
if
iH
xS2<occ.MinimumDepth())xS
if(score<2)xS
if(iZ*this,tree)eE==false)xS
if(l23*this,tree,synth,lL2)){y01}
if(!GoodMomentForCSE(*this,tree))xS
score*=tree
xS2;if(score>yP2){yP2=score;cs_it=i;}
}
if(yP2<=0){
#ifdef DEBUG_SUBSTITUTIONS_CSE
l13<<"No more CSE candidates.\n"
<<std::flush;
#endif
break;}
x53=cs_it
eA2
second;
#ifdef DEBUG_SUBSTITUTIONS_CSE
l13<<iM3"Common Subexpression:"
;DumpTree<yW>iH)xL1
l53
#endif
#if 0
int
lJ1=occ.NeedsSinCos();int
tS=occ.NeedsSinhCosh();xA
i92,iA2,yQ2,yR2;if(lJ1){i92
eV2
i92
cC
cSin);i92
tZ1
iA2
eV2
iA2
cC
cCos);iA2
tZ1
if(lM2(i92)||lM2(iA2)){if(lJ1==2){lL2
lN2
y01}
lJ1=0;}
}
if(tS){yQ2
eV2
yQ2
cC
cSinh);yQ2
tZ1
yR2
eV2
yR2
cC
cCosh);yR2
tZ1
if(lM2(yQ2)||lM2(yR2)){if(tS==2){lL2
lN2
y01}
tS=0;}
}
#endif
tree.SynthesizeByteCode(synth,false);lL2
lN2
#ifdef DEBUG_SUBSTITUTIONS_CSE
synth.nQ3
Dump<0>()xL1"Done with Common Subexpression:"
;DumpTree<yW>iH)xL1
l53
#endif
#if 0
if(lJ1){if(lJ1==2||tS){synth.tF1}
xF3
cSinCos,1,2
i11
i92,1
i11
iA2,0);}
if(tS){if(lJ1)synth.tF1
if(tS==2){synth.tF1}
xF3
cSinhCosh,1,2
i11
yQ2,1
i11
yR2,0);}
#endif
tD3
xE3
stacktop_before;}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
eQ1
l81
yW
lO2
using
t4;iT
xA
tree;tree.GenerateFrom(*mData);FPoptimizer_Optimize::ApplyGrammars
iH)e52
unsigned>yS3
e52
yW>immed;yU3
stacktop_max=0;tree.SynthesizeByteCode(yS3,immed,stacktop_max);if(mData->mStackSize!=stacktop_max){mData->mStackSize=unsigned(stacktop_max);
#if !defined(FP_USE_THREAD_SAFE_EVAL) && \
    !defined(FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA)
mData->mStack
nY3
stacktop_max);
#endif
}
mData->mByteCode.swap(yS3);mData->mImmed.swap(immed);}
#ifdef FP_SUPPORT_MPFR_FLOAT_TYPE
eT1>l81
MpfrFloat
lO2}
#endif
#ifdef FP_SUPPORT_GMP_INT_TYPE
eT1>l81
GmpInt
lO2}
#endif
FUNCTIONPARSER_INSTANTIATE_TYPES
#endif

#endif

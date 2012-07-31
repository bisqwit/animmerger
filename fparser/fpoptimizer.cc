/***************************************************************************\
|* Function Parser for C++ v4.5                                            *|
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
#include "fpaux.hh"
#define lP4 ==false
#define lO4 lN4 tX3
#define lN4 (*x5)[
#define lM4 yL3(cP*c61
#define lL4 =lK1 root l8
#define lK4 CalculateGroupFunction
#define lJ4 2)lS 2*
#define lI4 (lR)i82
#define lH4 tree.lU
#define lG4 cTan iO
#define lF4 :if lI
#define lE4 "Found "
#define lD4 ,cIf,cH3
#define lC4 stackpos
#define lB4 )const
#define lA4 sim.nY 1,
#define l94 (lB4{
#define l84 param.
#define l74 l84 data
#define l64 "dup(%u) "
#define l54 "%d, cost "
#define l44 "PUSH " e83
#define l34 "immed "<<
#define l24 mFuncParsers
#define l14 cZ2{assert
#define l04 stderr
#define iZ3 sep2=" "
#define iY3 FPHASH_CONST
#define iX3 cache_needed[
#define iW3 fprintf
#define iV3 "Applying "
#define iU3 ||tree.GetOpcode
#define iT3 HANDLE_UNARY_CONST_FUNC
#define iS3 size_t>
#define iR3 if(a>0){
#define iQ3 remaining
#define iP3 :start_at()
#define iO3 cW&start_at t53
#define iN3 FixIncompleteHashes()
#define iM3 ;size_t
#define iL3 ;}case
#define iK3 AddFrom(
#define iJ3 ;if(op==
#define iI3 second;
#define iH3 within,
#define iG3 eO i11)
#define iF3 c_count
#define iE3 s_count
#define iD3 MaxOp
#define iC3 tmp.yH1
#define iB3 tmp2.nJ
#define iA3 nP3 tQ2
#define i93 max.val
#define i83 )))tree
#define i73 eF2 if(
#define i63 cK p1);
#define i53 sim.x61
#define i43 b.Value)
#define i33 ByteCodeSynth xK
#define i23 b.Opcode
#define i13 ].swap(
#define i03 codes[b
#define tZ3 whydump
#define tY3 t63 lQ[b
#define tX3 0].info
#define tW3 *)&eC2;
#define tV3 nparams
#define tU3 if(!yN1
#define tT3 cLog iO
#define tS3 l4 0,1,
#define tR3 lA 0x12
#define tQ3 nQ 0,
#define tP3 cAbs nQ
#define tO3 y91 a),
#define tN3 {union{
#define tM3 false;}
#define tL3 nE1 cJ1
#define tK3 xF1++b)
#define tJ3 eO cU1);
#define tI3 c9 ifp2
#define tH3 );tmp eT1
#define tG3 (tree)!=
#define tF3 ].second
#define tE3 ].first
#define tD3 ){eJ xI=
#define tC3 cK r);}
#define tB3 Ne_Mask
#define tA3 Gt_Mask
#define t93 Lt_Mask
#define t83 NumConstant:
#define t73 switch(yF2.first iR2
#define t63 info.
#define t53 ,tB info
#define t43 opcode,
#define t33 resize(
#define t23 public:
#define t13 {data->
#define t03 n31 lH
#define eZ3 pclone
#define eY3 cOr,l6
#define eX3 newpow
#define eW3 lE2 1)
#define eV3 ,leaf1
#define eU3 eT3 nC1
#define eT3 lE2 a)
#define eS3 change
#define eR3 (count
#define eQ3 ,tree,info
#define eP3 133,2,
#define eO3 Needs
#define eN3 byteCode
#define eM3 lS1 nC==
#define eL3 cLog2by
#define eK3 x53 lV1
#define eJ3 factor_t
#define eI3 value1
#define eH3 Finite
#define eG3 ,0,0x16},{
#define eF3 =fp_pow(
#define eE3 FP_GetOpcodeName(
#define eD3 ){if(
#define eC3 max.known
#define eB3 tree xD
#define eA3 xD cMul);
#define e93 ParamHolder
#define e83 ;DumpTree(
#define e73 xD cond nC
#define e63 TreeCountItem
#define e53 );if(
#define e43 else{if(
#define e33 xK());nD
#define e23 p yN val
#define e13 c9 ifp1
#define e03 cAbsNot
#define cZ3 stackptr
#define cY3 cLog);xL
#define cX3 y4 cAdd)
#define cW3 for iJ nS
#define cV3 ;for iB1 a=
#define cU3 (l74.
#define cT3 .empty()
#define cS3 opcodes
#define cR3 did_muli
#define cQ3 xD data.
#define cP3 &Value){
#define cO3 yH const
#define cN3 used[b]
#define cM3 :{lX1 r=
#define cL3 std::tM2
#define cK3 sizeof(
#define cJ3 cAbsIf,
#define cI3 cNotNot,
#define cH3 l4 16,1,
#define cG3 18,2 eN1
#define cF3 l4 20,1,
#define cE3 l4 4,1,
#define cD3 450998,
#define cC3 cAtan2,
#define cB3 cExp2 nQ
#define cA3 lJ 2},0,
#define c93 if(p0 yN
#define c83 };enum
#define c73 1)tG1 1));
#define c63 ;}void
#define c53 xI)c63
#define c43 break;}
#define c33 middle2
#define c23 ::string
#define c13 default:
#define c03 n31;case
#define yZ3 tG));nV
#define yY3 switch(tM
#define yX3 range nQ3
#define yW3 range<n82 nT
#define yV3 range xK
#define yU3 Ge0Lt1
#define yT3 Gt0Le1
#define yS3 ==cAdd lR2
#define yR3 l8 2));
#define yQ3 l8 1))
#define yP3 l8 2)xC1
#define yO3 cAbsIf)
#define yN3 iterator
#define yM3 fp_equal(
#define yL3 &param=*
#define yK3 (constraints&
#define yJ3 ::vector<iG2>&xP1
#define yI3 ;if(yM3
#define yH3 ==cOr)l13
#define yG3 begin();
#define yF3 TreeSet
#define yE3 parent
#define yD3 insert(
#define yC3 newrel
#define yB3 void set
#define yA3 b_needed
#define y93 cachepos
#define y83 half=
#define y73 ,iB,1,l51+1);
#define y63 131,4,1,
#define y53 131,8,1,
#define y43 4,1,2,1,
#define y33 FindPos(
#define y23 src_pos
#define y13 reserve(
#define y03 tree.GetHash()
#define xZ3 lC2 tree
#define xY3 nJ1 void
#define xX3 treeptr
#define xW3 yZ2 iE,
#define xV3 std::endl;
#define xU3 xE lT 2,
#define xT3 std::cout
#define xS3 template lL
#define xR3 template lX
#define xQ3 ,cPow,
#define xP3 const std::eT
#define xO3 const char*
#define xN3 ;if(half
#define xM3 size()
#define xL3 tA1 void
#define xK3 ImmedTag
#define xJ3 a,const
#define xI3 RefCount
#define xH3 Birth();
#define xG3 typename
#define xF3 mulgroup
#define xE3 7168,
#define xD3 cost_t
#define xC3 fpdata
#define xB3 middle
#define xA3 result
#define x93 (cond yV
#define x83 tG1 1))){
#define x73 sqrt_cost
#define x63 const int
#define x53 xA3.
#define x43 n82(2)));
#define x33 maxValue1
#define x23 minValue1
#define x13 x03 known
#define x03 xA3 yN
#define nZ3 maxValue0
#define nY3 minValue0
#define nX3 ValueType
#define nW3 ;TriTruthValue
#define nV3 xA3 iL3
#define nU3 x03 val
#define nT3 xA3 tQ2
#define nS3 xA3 n51
#define nR3 xF3)
#define nQ3 xK xA3
#define nP3 )xA3
#define nO3 yK known
#define nN3 );m yN n3 0),
#define nM3 yK n3 0),
#define nL3 abs_mul
#define nK3 l8 a));
#define nJ3 pos_set
#define nI3 goto cY
#define nH3 subtree
#define nG3 invtree
#define nF3 MakeHash(
#define nE3 rulenumit
#define nD3 cAnd l3
#define nC3 282870 nP
#define nB3 cAnd,l6
#define nA3 MakeEqual
#define n93 MakeTrue,
#define n83 newbase
#define n73 branch1op
#define n63 branch2op
#define n53 overlap
#define n43 nE1 c9 y6
#define n33 truth_b
#define n23 truth_a
#define n13 IsLogicalValue y91
#define n03 &&p0 tQ2>=n82(0.0))
#define lZ3 ContainsOtherCandidates
#define lY3 if y91 0)
#define lX3 found_dup
#define lW3 );lC c6
#define lV3 cL1 eJ&
#define lU3 rangeutil
#define lT3 synth lN2
#define lS3 Plan_Has(
#define lR3 StackMax)
#define lQ3 const x82
#define lP3 namespace
#define lO3 ByteCode[
#define lN3 t2=!t2;}
#define lM3 inverted
#define lL3 IsNever:
#define lK3 .known&&
#define lJ3 iftree
#define lI3 :cAbsNotNot);
#define lH3 if(tZ1==
#define lG3 }switch(
#define lF3 depcodes
#define lE3 explicit
#define lD3 lB3 IsNever nV2
#define lC3 lB3 IsAlways;if(
#define lB3 ))return
#define lA3 .tL1 n]
#define l93 l2 16 eI
#define l83 l91 2,1,
#define l73 cCosh nQ
#define l63 n91 1,
#define l53 VarBegin
#define l43 Params[a].
#define l33 Params.
#define l23 ].data);
#define l13 ?0:1))l7
#define l03 c9 leaf2
#define iZ2 c9 leaf1
#define iY2 default_function_handling
#define iX2 sim.Eat(
#define iW2 *x5 n71,info
#define iV2 );tree tT2
#define iU2 );nV l5::
#define iT2 ,xA1);lC
#define iS2 ;xT3<<
#define iR2 ){case
#define iQ2 xD tZ1);
#define iP2 =true;lH2
#define iO2 .second)
#define iN2 begin(),
#define iM2 cond_add
#define iL2 cond_mul
#define iK2 cond_and
#define iJ2 func lL1
#define iI2 const eH
#define iH2 bool eA1
#define iG2 unsigned
#define iF2 costree
#define iE2 sintree
#define iD2 leaf_count
#define iC2 tG1-1)))xC
#define iB2 sub_params
#define iA2 printf(
#define i92 nR){n82 tmp=
#define i82 );nD lC
#define i72 swap(tmp);
#define i62 cbrt_count
#define i52 sqrt_count
#define i42 PlusInf
#define i32 ,xT3
#define i22 pcall_tree
#define i12 after_powi
#define i02 GetHash().
#define tZ2 ),child);
#define tY2 ,bool abs)
#define tX2 ;(*x5 n71=r.specs;if(r.found){
#define tW2 TopLevel)
#define tV2 .what nU1
#define tU2 (half&63)-1;
#define tT2 .DelParam(
#define tS2 bool t2=false;
#define tR2 if y91 1)i71&&
#define tQ2 .min.val
#define tP2 else{x5=new
#define tO2 (x53 min.
#define tN2 ,o);o<<"\n";
#define tM2 vector<bool>
#define tL2 eQ1=0;a<xV;++a)
#define tK2 eQ1;if(&eC2){x5=(
#define tJ2 a<xV;++a){
#define tI2 .n_int_sqrt
#define tH2 ){eJ r;r xD
#define tG2 ,eU,synth);
#define tF2 ;break;
#define tE2 (leaf1 l8
#define tD2 (xF3
#define tC2 (yK val
#define tB2 (exponent
#define tA2 )tF2}
#define t92 cN1.SubTrees
#define t82 cN1.Others
#define t72 cN1.Immeds
#define t62 yR tM3
#define t52 grammar
#define t42 cCos iO
#define t32 0x12},{{3,
#define t22 ,tree nX
#define t12 cNeg,lT 1,
#define t02 ,cEqual eW1
#define eZ2 ),0},{
#define eY2 data;data.
#define eX2 MakeNEqual
#define eW2 Dump(std::
#define eV2 isInteger(
#define eU2 Comparison
#define eT2 needs_flip
#define eS2 value]
#define eR2 ~size_t(0)
#define eQ2 xJ1 xS+1);
#define eP2 Rule&rule,
#define eO2 )lS 3*3*
#define eN2 >::res,b8<
#define eM2 continue;}
#define eL2 c9 tree);
#define eK2 mul_item
#define eJ2 innersub
#define eI2 cbrt_cost
#define eH2 best_cost
#define eG2 fp_min(yL
#define eF2 tree t31);}
#define eE2 tree))cZ
#define eD2 condition
#define eC2 *start_at
#define eB2 per_item
#define eA2 item_type
#define e92 first2
#define e82 l4 18,1,
#define e72 lJ 1},0,
#define e62 Decision
#define e52 not_tree
#define e42 group_by
#define e32 exponent=
#define e22 ->second
#define e12 ,eL1 0x1 nH
#define e02 ;iB.Remember(
#define cZ2 eJ&tree)
#define cY2 targetpos
#define cX2 ParamSpec
#define cW2 rhs.hash2;}
#define cV2 rhs.hash1
#define cU2 struct
#define cT2 Forget()
#define cS2 &&cond e8))
#define cR2 source_tree
#define cQ2 nC==cLog2&&
#define cP2 )continue
#define cO2 <tW,xD3>
#define cN2 CodeTree lW
#define cM2 p1_evenness
#define cL2 (tK==eH3&&
#define cK2 isNegative(
#define cJ2 yN known&&(
#define cI2 neg_set
#define cH2 cNop,cNop}}
#define cG2 cTanh,cNop,
#define cF2 NewHash
#define cE2 >cU2 cA<
#define cD2 matches
#define cC2 .match_tree
#define cB2 );sim.nY 2,
#define cA2 cL1 void*)&
#define c92 cSin iO
#define c82 cTan nQ
#define c72 ,cLog nQ
#define c62 cCos nQ
#define c52 long value
#define c42 (std::move(
#define c32 (xX l8 a)xF
#define c22 negated
#define c12 Specializer
#define c02 params
#define yZ2 18,2,
#define yY2 coshtree
#define yX2 sinhtree
#define yW2 best_score
#define yV2 mulvalue
#define yU2 pow_item
#define yT2 subgroup
#define yS2 nC==cPow&&tO
#define yR2 PowiResult
#define yQ2 )))t03
#define yP2 0));yV3
#define yO2 maxValue
#define yN2 minValue
#define yM2 div_tree
#define yL2 pow_tree
#define yK2 preserve
#define yJ2 PullResult()
#define yI2 dup_or_fetch
#define yH2 nominator]
#define yG2 test_order
#define yF2 parampair
#define yE2 yF2,
#define yD2 .param_count
#define yC2 minimum_need
#define yB2 shift(index)
#define yA2 {xT3<<
#define y92 rulenumber
#define y82 l2 16,2,
#define y72 cTanh nQ
#define y62 xQ3 l2 2,2,
#define y52 ,cPow iO
#define y42 cSinh nQ
#define y32 cIf,n91 3,
#define y22 cInv,lT 1,
#define y12 constraints=
#define y02 GetDepth()
#define xZ2 factor_immed
#define xY2 changes
#define xX2 c9 cond l8
#define xW2 Become y91
#define xV2 for(xG3
#define xU2 exp_diff
#define xT2 ExponentInfo
#define xS2 )){data x7 lN
#define xR2 lower_bound(
#define xQ2 factor
#define xP2 is_logical
#define xO2 newrel_and
#define xN2 Suboptimal
#define xM2 eK[c eA
#define xL2 res_stackpos
#define xK2 half_pos
#define xJ2 i8 push_back(
#define xI2 ifdata.ofs
#define xH2 n31 true;}
#define xG2 >>1)):(
#define xF2 CodeTreeData
#define xE2 exponent)
#define xD2 yN known)
#define xC2 multiply(
#define xB2 )y4 cPow)
#define xA2 var_trees
#define x92 nB OPCODE
#define x82 CodeTree&
#define x72 parent_opcode
#define x62 GetParam(a eS
#define x52 {cW start_at;
#define x42 changed=true;
#define x32 log2_exponent
#define x22 lR,tH)i82
#define x12 dup_fetch_pos
#define x02 IsNever cL lC
#define nZ2 cSin nQ
#define nY2 Value_EvenInt
#define nX2 AddCollection
#define nW2 ConditionType
#define nV2 n31 Unknown;}
#define nU2 1 y7 i8 size(
#define nT2 (iG2
#define nS2 iA|nT2)
#define nR2 SpecialOpcode
#define nQ2 =i e22.
#define nP2 yN known&&p
#define nO2 assimilated
#define nN2 :sim n92
#define nM2 denominator
#define nL2 fraction
#define nK2 DUP_BOTH();
#define nJ2 0x80000000u
#define nI2 IsDescendantOf
#define nH2 TreeCounts
#define nG2 SetOpcode(
#define nF2 found_log2
#define nE2 div_params
#define nD2 immed_sum
#define nC2 ;sim.Push(
#define nB2 ,cMul l3
#define nA2 lO3++IP]
#define n92 .Eat(1,
#define n82 Value_t
#define n72 OPCODE(opcode)
#define n62 FactorStack xK
#define n52 )iS2
#define n42 Rehash(false);
#define n32 IsAlways cL lC
#define n22 cEqual,
#define n12 cNotNot nQ
#define n02 cNot nQ
#define lZ2 DumpHashesFrom
#define lY2 replacing_slot
#define lX2 RefParams
#define lW2 if_always[
#define lV2 WhatDoWhenCase
#define lU2 exponent_immed
#define lT2 new_base_immed
#define lS2 base_immed
#define lR2 ||op1==
#define lQ2 l8 0));
#define lP2 data[a tF3
#define lO2 if(newrel_or==
#define lN2 .AddOperation(
#define lM2 DUP_ONE(apos);
#define lL2 flipped
#define lK2 .UseGetNeeded(
#define lJ2 ,fp_max(yL);
#define lI2 else if(
#define lH2 lI2!xA3
#define lG2 synth.PushImmed(
#define lF2 ,PowiCache&iB,
#define lE2 tree l8
#define lD2 <<tree.i02
#define lC2 const eJ&
#define lB2 ,l1 0x0},{{3,
#define lA2 ,0,0x4},{{1,
#define l92 {case IsAlways:
#define l82 e3 2,131,
#define l72 (IfData&ifdata
#define l62 [xS-1-offset].
#define l52 lO3 a
#define l42 Immed.xM3);
#define l32 OptimizedUsing
#define l22 Var_or_Funcno
#define l12 l22;
#define l02 GetParams(
#define iZ1 crc32_t
#define iY1 signed_chain
#define iX1 MinusInf
#define iW1 n_immeds
#define iV1 stack.xM3
#define iU1 FindClone(xI
#define iT1 lO3 IP]
#define iS1 GetOpcode())
#define iR1 needs_rehash
#define iQ1 AnyWhere_Rec
#define iP1 ~iG2(0)
#define iO1 41,42,43,44,
#define iN1 p1_logical_b
#define iM1 p0_logical_b
#define iL1 p1_logical_a
#define iK1 p0_logical_a
#define iJ1 *const func)
#define iI1 synth.DoDup(
#define iH1 cache_needed
#define iG1 e3 2,1,e3 2,
#define iF1 treelist
#define iE1 has_bad_balance
#define iD1 {if(GetOpcode()
#define iC1 if(iQ3[a])
#define iB1 (size_t
#define iA1 for iB1 b=0;b<
#define i91 for iB1 a y1
#define i81 }c43 case
#define i71 .IsImmed()
#define i61 i71 eD3
#define i51 eJ3 xQ2
#define i41 fp_abs(i93))
#define i31 fp_abs(min.val)
#define i21 template<
#define i11 divgroup
#define i01 2*2*2)lS 3
#define tZ1 tree nC
#define tY1 xR3 void
#define tX1 synth.xM 1
#define tW1 ,iB tG2
#define tV1 if(&*(*x5 n71){
#define tU1 tJ 2},0,0x0},{{
#define tT1 Oneness_NotOne|
#define tS1 Value_IsInteger
#define tR1 Constness_Const
#define tQ1 ;xI.SetParamsMove(
#define tP1 l32(
#define tO1 reltype
#define tN1 SequenceOpcodes
#define tM1 synth.Find(
#define tL1 sep_list[
#define tK1 TreeCountType xK
#define tJ1 nH2.erase(cs_it);
#define tI1 lB3 true
#define tH1 yK set(fp_floor);
#define tG1 ,n82(
#define tF1 goto fail;}
#define tE1 );exponent
#define tD1 back().thenbranch
#define tC1 grammar_rules[*r]
#define tB1 l1 0x4 nH
#define tA1 lX2);
#define t91 IsDefined()){
#define t81 eJ tmp;tmp xD
#define t71 >(n82(1),
#define t61 lN4 a].info
#define t51 n82(0.0)){xO
#define t41 DelParam(a);}
#define t31 tT2 a
#define t21 a)i71)
#define t11 n72);
#define t01 0.5)xB2;lC
#define eZ1 xT3<<"POP "
#define eY1 stack[iV1-
#define eX1 stack.push_back(
#define eW1 ,l0 2,
#define eV1 CollectionSet xK
#define eU1 xK())y4 cMul);lC
#define eT1 .Rehash();
#define eS1 ParsePowiMuli
#define eR1 MaxChildDepth
#define eQ1 iG2 a
#define eP1 std::pair<It,It>
#define eO1 cLess,l2
#define eN1 ,nC3 281856,
#define eM1 cNEqual,
#define eL1 cPow,lA
#define eK1 Sign_Negative
#define eJ1 Value_Logical
#define eI1 yD MakeFalse,{l5
#define eH1 new_factor_immed
#define eG1 occurance_pos
#define eF1 exponent_hash
#define eE1 exponent_list
#define eD1 CollectMulGroup(
#define eC1 source_set
#define eB1 exponent,yF3
#define eA1 operator
#define e91 FindAndDup(tree);
#define e81 ParamSpec_Extract
#define e71 retry_anyparams_3
#define e61 retry_anyparams_2
#define e51 needlist_cached_t
#define e41 lJ 2}lA2
#define e31 lJ 1}lA2
#define e21 CodeTreeImmed xK(
#define e11 by_float_exponent
#define e01 fp_equal tB2
#define cZ1 new_exp
#define cY1 end()&&i->first==
#define cX1 return BecomeZero;
#define cW1 return BecomeOne;
#define cV1 if(lQ.xM3<=n1)
#define cU1 addgroup
#define cT1 found_log2by
#define cS1 nC==e03)
#define cR1 if(keep_powi
#define cQ1 l22)
#define cP1 branch1_backup
#define cO1 branch2_backup
#define cN1 NeedList
#define cM1 ;synth.StackTopIs(
#define cL1 (const
#define cK1 cL1 n82&
#define cJ1 eT1 eB3 op1);tree.DelParams()
#define cI1 .GetParamCount()==
#define cH1 tree.SetParam(
#define cG1 exponent_map
#define cF1 plain_set
#define cE1 rangehalf
#define cD1 LightWeight(
#define cC1 if(value
#define cB1 xR3 yX
#define cA1 xR3 static
#define c91 m yN set(fp_ceil);tV
#define c81 m yN val
#define c71 m yN known
#define c61 )yF2.iI3
#define c51 ::MakeTrue
#define c41 should_regenerate=true;
#define c31 should_regenerate,
#define c21 Collection
#define c11 RelationshipResult
#define c01 Subdivide_Combine(
#define yZ1 lB4 yR
#define yY1 rhs yZ1 hash1
#define yX1 PositionalParams,0
#define yW1 best_sep_factor
#define yV1 needlist_cached
#define yU1 inline iG2
#define yT1 252421 nP 24830
#define yS1 t43 bool pad
#define yR1 MakesInteger(
#define yQ1 xF3.
#define yP1 .AddParamMove(
#define yO1 =comp.AddItem(atree
#define yN1 tree i71 cL
#define yM1 i33&synth)
#define yL1 lP3 FPoptimizer_Optimize
#define yK1 (long double)
#define yJ1 const n82&value
#define yI1 best_sep_cost
#define yH1 AddParamMove(tree);
#define yG1 MultiplicationRange
#define yF1 pihalf_limits
#define yE1 n_stacked
#define yD1 cF2.hash1
#define yC1 AnyParams_Rec
#define yB1 ):e2(),std::vector<
#define yA1 ;DumpTree cJ
#define y91 (lE2
#define y81 Become(value l8 0))
#define y71 always_sincostan
#define y61 Recheck_RefCount_Div
#define y51 Recheck_RefCount_Mul
#define y41 xF3;xF3 xD
#define y31 MultiplyAndMakeLong(
#define y21 n82(0)
#define y11 covers_plus1
#define y01 if(synth.FindAndDup(
#define xZ1 SynthesizeParam(
#define xY1 grammar_func
#define xX1 cOr l3 16,1,
#define xW1 252180 nP 281854
#define xV1 l2 0,2,165888 nP
#define xU1 l1 0x12 nH
#define xT1 Modulo_Radians},
#define xS1 PositionType
#define xR1 CollectionResult
#define xQ1 ;m yN template set_if<
#define xP1 ByteCode,size_t&IP,size_t limit,size_t y2
#define xO1 (xA3))break;xA3*=
#define xN1 xR3 bool
#define xM1 const_offset
#define xL1 inline TriTruthValue
#define xK1 stacktop_desired
#define xJ1 SetStackTop(
#define xI1 }inline
#define xH1 FPoptimizer_ByteCode
#define xG1 1)?(poly^(
#define xF1 GetParamCount();
#define xE1 public e2,public std::vector<
#define xD1 y21)
#define xC1 xF leaf2 l8
#define xB1 yZ2 529654 nP
#define xA1 cond_type
#define x91 fphash_value_t
#define x81 Recheck_RefCount_RDiv
#define x71 cMul);tmp.nJ 0));tmp.
#define x61 SwapLastTwoInStack();
#define x51 fPExponentIsTooLarge(
#define x41 CollectMulGroup_Item(
#define x31 pair<n82,yF3>
#define x21 nL xJ1 xS-1);
#define x11 covers_full_cycle
#define x01 AssembleSequence(
#define nZ1 <<std::dec<<")";}
#define nY1 yD MakeNotP1,l5::
#define nX1 yD MakeNotP0,l5::
#define nW1 {DataP slot_holder(xY[
#define nV1 :return p tQ2
#define nU1 !=xB)if(TestCase(
#define nT1 &&IsLogicalValue(
#define nS1 std::pair<T1,T2>&
#define nR1 i21 xG3
#define nQ1 has_good_balance_found
#define nP1 n_occurrences
#define nO1 found_log2_on_exponent
#define nN1 covers_minus1
#define nM1 needs_resynth
#define nL1 immed_product
#define nK1 ,2,1 xN if(found[data.
#define nJ1 ;xR3
#define nI1 break;lG3 bitmask&
#define nH1 Sign_Positive
#define nG1 SetParamMove(
#define nF1 CodeTreeImmed(n82(
#define nE1 changed_if
#define nD1 0,xE2;DelParam(1);
#define nC1 .GetImmed()
#define nB1 for iB1 a=0;a<c8++a)
#define nA1 iG2 index
#define n91 lA 0x4},{{
#define n81 iI1 found[data.
#define n71 )[a].start_at
#define n61 ,cIf,l0 3,
#define n51 .min.known
#define n41 .xF1 a-->0;)if(
#define n31 ;return
#define n21 n31 xN2;}
#define n11 >::Optimize(){}
#define n01 n_as_tanh_param
#define lZ1 opposite=
#define lY1 x91(
#define lX1 MatchResultType
#define lW1 needs_sincos
#define lV1 resulting_exponent
#define lU1 val):Value(Value::
#define lT1 Unknown:c13;}
#define lS1 GetParam(a)
#define lR1 inverse_nominator]
#define lQ1 cMul l3 0,1,
#define lP1 tree yP1
#define lO1 AddFunctionOpcode(
#define lN1 SetParams(l02));
#define lM1 o<<"("<<std::hex<<data.
#define lL1 (val);else*this=model;}
#define lK1 IfBalanceGood(
#define lJ1 n_as_tan_param
#define lI1 changed_exponent
#define lH1 &&e23<n82(
#define lG1 inverse_denominator
#define lF1 ;cF2.hash2+=
#define lE1 xK(rule.repl_param_list,
#define lD1 retry_positionalparams_2
#define lC1 situation_flags&
#define lB1 518 nP 400412,
#define lA1 7168 nP 401798
#define l91 }},{ProduceNewTree,
#define l81 data.subfunc_opcode
#define l71 i71){n82
#define l61 CopyOnWrite();
#define l51 recursioncount
#define l41 PlanNtimesCache(
#define l31 >){int mStackPtr=0;
#define l21 FPoptimizer_Grammar
#define l11 AddOperation(cInv,1,1 xN}
#define l01 GetPositivityInfo tG3
#define iZ ParamSpec_SubFunctionData
#define iY iB1 a=c8 a-->0;)
#define iX PositionalParams_Rec
#define iW ,cMul x4
#define iV eT1 eB3 leaf1 nC);tree.
#define iU yD MakeNotNotP1,l5::
#define iT yD MakeNotNotP0,l5::
#define iS lO3 xI2+
#define iR DumpTreeWithIndent(*this);
#define iQ switch(type iR2 cond_or:
#define iP ;if(tW2 t63 SaveMatchedParamIndex(
#define iO ,l4 2,1,
#define iN CalculateResultBoundaries(
#define iM i21 iG2 Compare>
#define iL cA3 0x0},{{1,
#define iK edited_powgroup
#define iJ iB1 a=xF1 a
#define iI has_unknown_max
#define iH has_unknown_min
#define iG static const yV3
#define iF synthed_tree
#define iE 408964 nP 24963
#define iD SelectedParams,0},0,0x0},{{
#define iC collections
#define iB cache
#define iA ;xJ2 nJ2
#define i9 )iA);
#define i8 ByteCode.
#define i7 ;pow xD cLog);tree eA3
#define i6 goto ReplaceTreeWithOne;case
#define i5 ]);lT3
#define i4 !=xB)return lW2
#define i3 e11.data
#define i2 lE3 xF2(
#define i1 needs_sinhcosh
#define i0 cAdd x4
#define tZ cAdd l3 0,
#define tY eZ2 n82(
#define tX xR3 nA
#define tW int_exponent_t
#define tV return m iL3
#define tU MakeFalse,l5::
#define tT n52 xV3 DumpHashes(
#define tS ,ByteCode,IP,limit,y2,stack);
#define tR matched_params
#define tQ [n1 tE3=true;lQ[n1 tF3
#define tP l21::Grammar*
#define tO powgroup l8
#define tN );p2 cK p2);eB3 lJ3 nC);cZ}
#define tM GetLogicalValue y91
#define tL eR2&&found[data.
#define tK valueType
#define tJ xE AnyParams,
#define tI =iN lE2
#define tH eW3 nC1
#define tG (tH)
#define tF nF1(
#define tE has_mulgroups_remaining
#define tD by_exponent
#define tC const iZ
#define tB MatchInfo xK&
#define tA Rehash();iB2.push_back(
#define t9 best_factor
#define t8 RootPowerTable xK::RootPowers[
#define t7 MatchPositionSpec_AnyParams xK
#define t6 cLessOrEq,l2
#define t5 lP3 FPoptimizer_CodeTree
#define t4 n_as_sinh_param
#define t3 n_as_cosh_param
#define t2 is_signed
#define t1 result_positivity
#define t0 yN known=false;
#define eZ biggest_minimum
#define eY 142455 nP 141449,
#define eX cond_tree
#define eW else_tree
#define eV then_tree
#define eU sequencing
#define eT string eE3
#define eS );bool needs_cow=GetRefCount()>1;
#define eR nM n82(-c73
#define eQ {AdoptChildrenWithSameOpcode(tree);
#define eP ,cGreater,l2 yZ2
#define eO ;AddParamMove(
#define eN yN known&&p0 yN val<=fp_const_negativezero xK())
#define eM tree.GetParamCount()
#define eL :goto ReplaceTreeWithZero;case
#define eK relationships
#define eJ CodeTree xK
#define eI ,2,122999 nP 139399,
#define eH std::vector<eJ>
#define eG if_stack
#define eF (l02));yQ1 Rehash();
#define eE n_as_sin_param
#define eD n_as_cos_param
#define eC PowiResolver::
#define eB cIf,tS3
#define eA ].relationship
#define e9 PACKED_GRAMMAR_ATTRIBUTE;
#define e8 .BalanceGood
#define e7 AddParamMove(yT2
#define e6 back().endif_location
#define e5 x91 key
#define e4 AddParamMove(mul);
#define e3 130,1,
#define e2 MatchPositionSpecBase
#define e1 lE3 CodeTree(
#define e0 smallest_maximum
#define cZ goto redo;
#define cY ReplaceTreeWithParam0;
#define cX factor_needs_rehashing
#define cW MatchPositionSpecBaseP
#define cV xG3 tK1::yN3
#define cU e81 xK(nN.param_list,
#define cT 243,244,245,246,249,250,251,253,255,256,257,258,259}};}
#define cS ];};extern"C"{
#define cR 79,122,123,160,161,163,164,165,166,167,168,169,178,179,180,200,204,212,216,224,236,237,239,240,
#define cQ 27,28,29,30,31,32,33,35,36,
#define cP const ParamSpec_SubFunction
#define cO const ParamSpec_ParamHolder
#define cN otherhalf
#define cM StackState
#define cL )return false;
#define cK eT1 lP1
#define cJ (tree n52"\n";
#define cI const SequenceOpCode xK
#define cH paramholder_matches[nZ]
#define cG MatchPositionSpec_PositionalParams xK
#define cF xZ3,std::ostream&o
#define cE paramholder_matches.
#define cD n82(1.5)*fp_const_pi xK()
#define cC xB,l5::Never yD xB,l5::Never}
#define cB CalculatePowiFactorCost(
#define cA ImmedHashGenerator
#define c9 .AddParam(
#define c8 eM;
#define c7 ::map<fphash_t,std::set<std c23> >
#define c6 ComparisonSetBase::
#define c5 AddParamMove(comp.cF1[a].value);
#define c4 ,y82 528503 nP 24713,
#define c3 T1,xG3 T2>inline iH2()(
#define c2 has_nonlogical_values
#define c1 from_logical_context)
#define c0 AnyParams,0 l91
#define yZ for iB1 a=xX.xF1 a-->0;)
#define yY POWI_CACHE_SIZE
#define yX static inline eJ
#define yW ++IP;eM2 if(iT1==cS3.
#define yV .FoundChild
#define yU BalanceResultType
#define yT xI3(0),Opcode(
#define yS );void lO1 iG2 t43 c12<
#define yR {return
#define yQ const yR data->
#define yP +=fp_const_twopi xK();
#define yO fp_const_twopi xK()e53
#define yN .max.
#define yM eJ tmp,tmp2;tmp2 xD
#define yL fp_sin(min),fp_sin(max))
#define yK m.min.
#define yJ lN2 GetOpcode(),
#define yI for iB1 a=0;a<xF1++a eD3
#define yH static void nF3 nB fphash_t&cF2,
#define yG MatchPositionSpec_AnyWhere
#define yF if cU3 match_type==
#define yE void OutFloatHex(std::ostream&o,
#define yD },{l5::
#define yC AddParam(CodeTreeImmed(
#define yB cGreaterOrEq,
#define yA ,xG3 eJ::
#define y9 xK model=cE1 xK()eD3 known
#define y8 AssembleSequence_Subdivide(
#define y7 ]=nJ2|iG2(
#define y6 branch2
#define y5 !=eR2){n81
#define y4 ;iX2 2,
#define y3 iG2 c;iG2 short l[
#define y2 factor_stack_base
#define y1 =0;a<yE3.xF1++a)if(
#define y0 (nE3 r=range.first;r!=range.iI3++r){
#define xZ ,eL1 0x4 nH
#define xY data->Params
#define xX branch1
#define xW );eM2 if(list.first nC1==n82(
#define xV nN yD2
#define xU =fp_cosh tC2);c81=fp_cosh(c81);
#define xT {nH2.erase(i);eM2
#define xS StackTop
#define xR FPOPT_autoptr
#define xQ +=xA3 n31 xA3;}xR3 inline n82
#define xP int_exponent
#define xO tree.ReplaceWithImmed(
#define xN )cM1*this)n31;}
#define xM GetStackTop()-
#define xL sim.AddConst(
#define xK <n82>
#define xJ .SetParam(0,lJ3 lQ2 eJ p1;p1 xD
#define xI newnode
#define xH has_highlevel_opcodes
#define xG e72 0x0},{{
#define xF .IsIdenticalTo(
#define xE ,cAdd,
#define xD .nG2
#define xC {if(needs_cow){l61 goto
#define xB Unchanged
#define xA cF=xT3
#define x9 best_selected_sep
#define x8 i21>void FunctionParserBase<
#define x7 ->Recalculate_Hash_NoRecursion();}
#define x6 xF1++a)if(ApplyGrammar(t52,eT3,
#define x5 position
#define x4 l3 2,1,
#define x3 ;iB3 0));tmp xD cInv);tmp yP1 tmp2)n31
#define x2 nB1{yV3
#define x1 std::vector<CodeTree>
#define x0 TestImmedConstraints(l84 constraints,tree)cL
#define nZ paramholder_index
#define nY x61 iX2
#define nX )){tree.iN3;}
#define nW );cR1){sim n92 cInv tA2 xL-1 xB2;lC
#define nV return true;case
#define nU occurance_counts
#define nT >p tI a)e53 p.
#define nS -->0;){lC2 powgroup=lS1;if(powgroup
#define nR lY3 i71
#define nQ ,l0 1,
#define nP ,{2,
#define nO const FPoptimizer_CodeTree::eJ&tree
#define nN model_tree
#define nM return yV3(
#define nL ){using lP3 FUNCTIONPARSERTYPES;
#define nK eH&lX2
#define nJ AddParam y91
#define nI ConstantFolding_LogicCommon(tree,c6
#define nH },{{2,
#define nG nR1 Ref>inline void xR<Ref>::
#define nF AnyParams,1},0,0x0},{{
#define nE ):data(new xF2 xK(
#define nD goto do_return;}
#define nC .GetOpcode()
#define nB FUNCTIONPARSERTYPES::
#define nA xF2 xK::xF2(
#define n9 b;}};i21>cU2 Comp<nB
#define n8 l22(),Params(),Hash(),Depth(1),tP1 0){}
#define n7 SynthesizeByteCode(synth);
#define n6 while(ApplyGrammar(cA2
#define n5 GetIntegerInfo y91 0))==IsAlways)nI3
#define n4 ;lP1 nE1)xH2
#define n3 template set_if<cGreater>(n82(
#define n2 DumpParams xK cU3 param_list,l74 yD2,o);
#define n1 restholder_index
#define n0 eJ exponent;exponent xD cMul tE1 c9
#define lZ lR e53 fp_nequal(tmp,xD1){xO n82(1)/tmp);nD}lC
#define lY :if(ParamComparer xK()(Params[1],Params[0])){std::swap(Params[0],Params[1]);Opcode=
#define lX <xG3 n82>
#define lW xK tmp;tmp xD cPow);tmp.nJ 0));tmp.yC n82(
#define lV tR1,0x0},
#define lU AddParamMove(pow yQ3;pow tT2 1);pow eT1 tree.nG1 0,pow);goto NowWeAreMulGroup;}
#define lT GroupFunction,0},lV{{
#define lS tG1 1)/n82(
#define lR lE2 0)nC1
#define lQ restholder_matches
#define lP yD1|=key;x91 crc=(key>>10)|(key<<(64-10))lF1((~lY1 crc))*3)^1234567;}};
#define lO nE1;nE1 iQ2 nE1.AddParamMove y91 0));nE1 c9 xX l8
#define lN xR3 eJ::CodeTree(
#define lM cH1 0,lE2 0)lQ2 cH1 1,CodeTreeImmed(
#define lL lX void i33::lO1 iG2 t43 c12<
#define lK cMul,lT 2,
#define lJ cMul,AnyParams,
#define lI y91 0)i71&&eW3 i71){xO
#define lH iN tmp)iL3
#define lG :eS3=comp.AddRelationship(atree l8 0),atree l8 1),c6
#define lF cPow,l0 2
#define lE xG3 n82>inline iH2()cK1 xJ3 n82&b)yR a
#define lD {yV3 m tI 0));
#define lC break;case
#define lB tY1 eJ::
#define lA yX1},0,
#define l9 l1 0x0 nH
#define l8 .GetParam(
#define l7 ;eJ nE1;nE1 iQ2 nE1.SetParamsMove(tree.l02));nE1 eT1 eB3
#define l6 SelectedParams,0},0,0x0 nH
#define l5 RangeComparisonData
#define l4 yX1 l91
#define l3 ,AnyParams,0}},{ReplaceParams,
#define l2 yX1}},{ReplaceParams,
#define l1 cMul,SelectedParams,0},0,
#define l0 lA 0x0},{{
#ifdef _MSC_VER
typedef
iG2
int
iZ1;
#else
#include <stdint.h>
typedef
uint_least32_t
iZ1;
#endif
lP3
crc32{enum{startvalue=0xFFFFFFFFUL,poly=0xEDB88320UL}
;i21
iZ1
crc>cU2
b8{enum{b1=(crc&xG1
crc
xG2
crc>>1),b2=(b1&xG1
b1
xG2
b1>>1),b3=(b2&xG1
b2
xG2
b2>>1),b4=(b3&xG1
b3
xG2
b3>>1),b5=(b4&xG1
b4
xG2
b4>>1),b6=(b5&xG1
b5
xG2
b5>>1),b7=(b6&xG1
b6
xG2
b6>>1),res=(b7&xG1
b7
xG2
b7>>1)}
;}
;inline
iZ1
update(iZ1
crc,iG2
b){
#define B4(n) b8<n eN2 n+1 eN2 n+2 eN2 n+3>::res
#define R(n) B4(n),B4(n+4),B4(n+8),B4(n+12)
static
const
iZ1
table[256]={R(0x00),R(0x10),R(0x20),R(0x30),R(0x40),R(0x50),R(0x60),R(0x70),R(0x80),R(0x90),R(0xA0),R(0xB0),R(0xC0),R(0xD0),R(0xE0),R(0xF0)}
;
#undef R
#undef B4
return((crc>>8))^table[(crc^b)&0xFF];xI1
iZ1
calc_upd(iZ1
c,const
iG2
char*buf,size_t
size){iZ1
value=c;for
iB1
p=0;p<size;++p)value=update(value,buf[p])n31
value;xI1
iZ1
calc
cL1
iG2
char*buf,size_t
size)yR
calc_upd(startvalue,buf,size);}
}
#ifndef FPOptimizerAutoPtrHH
#define FPOptimizerAutoPtrHH
nR1
Ref>class
xR{t23
xR():p(0){}
xR(Ref*b):p(b){xH3}
xR
cL1
xR&b):p(b.p){xH3
xI1
Ref&eA1*(yZ1*p;xI1
Ref*eA1->(yZ1
p;}
xR&eA1=(Ref*b){Set(b)n31*this;}
xR&eA1=cL1
xR&b){Set(b.p)n31*this;}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
xR(xR&&b):p(b.p){b.p=0;}
xR&eA1=(xR&&b
eD3
p!=b.p){cT2;p=b.p;b.p=0;}
return*this;}
#endif
~xR(){cT2
c63
UnsafeSetP(Ref*newp){p=newp
c63
swap(xR<Ref>&b){Ref*tmp=p;p=b.p;b.p=tmp;}
private:inline
static
void
Have(Ref*p2);inline
void
cT2;inline
void
xH3
inline
void
Set(Ref*p2);private:Ref*p;}
;nG
cT2{if(!p)return;p->xI3-=1;if(!p->xI3)delete
p;}
nG
Have(Ref*p2
eD3
p2)++(p2->xI3);}
nG
Birth(){Have(p);}
nG
Set(Ref*p2){Have(p2);cT2;p=p2;}
#endif
#include <utility>
cU2
Compare2ndRev{nR1
T>inline
iH2()cL1
T&xJ3
T&b
yZ1
a.second>b.iI3}
}
;cU2
Compare1st{nR1
c3
const
nS1
xJ3
nS1
b
yZ1
a.first<b.first;}
nR1
c3
const
nS1
a,T1
b
yZ1
a.first<b;}
nR1
c3
T1
xJ3
nS1
b
yZ1
a<b.first;}
}
;
#ifndef FPoptimizerHashHH
#define FPoptimizerHashHH
#ifdef _MSC_VER
typedef
iG2
long
long
x91;
#define FPHASH_CONST(x) x##ULL
#else
#include <stdint.h>
typedef
uint_fast64_t
x91;
#define FPHASH_CONST(x) x##ULL
#endif
lP3
FUNCTIONPARSERTYPES{cU2
fphash_t{x91
hash1,hash2;fphash_t():hash1(0),hash2(0){}
fphash_t
cL1
x91&xJ3
x91&b):hash1(a),hash2(b){}
iH2==cL1
fphash_t&yY1==cV2&&hash2==cW2
iH2!=cL1
fphash_t&yY1!=cV2||hash2!=cW2
iH2<cL1
fphash_t&yY1!=cV2?hash1<cV2:hash2<cW2}
;}
#endif
#ifndef FPOptimizer_CodeTreeHH
#define FPOptimizer_CodeTreeHH
#ifdef FP_SUPPORT_OPTIMIZER
#include <vector>
#include <utility>
lP3
l21{cU2
Grammar;}
lP3
xH1{xR3
class
ByteCodeSynth;}
t5{xR3
class
CodeTree
nJ1
cU2
xF2
nJ1
class
CodeTree{typedef
xR<xF2
xK>DataP;DataP
data;t23
CodeTree();~CodeTree();cU2
OpcodeTag{}
;e1
x92
o,OpcodeTag);cU2
FuncOpcodeTag{}
;e1
x92
o,iG2
f,FuncOpcodeTag);cU2
xK3{}
;e1
const
n82&v,xK3);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
e1
n82&&v,xK3);
#endif
cU2
VarTag{}
;e1
iG2
varno,VarTag);cU2
CloneTag{}
;e1
lQ3
b,CloneTag);void
GenerateFrom
cL1
xG3
FunctionParserBase
xK::Data&data,bool
keep_powi=false);void
GenerateFrom
cL1
xG3
FunctionParserBase
xK::Data&data,const
x1&xA2,bool
keep_powi=false);void
SynthesizeByteCode(std::vector<iG2>&eN3,std::vector
xK&immed,size_t&stacktop_max);void
SynthesizeByteCode(xH1::i33&synth,bool
MustPopTemps=true
lB4
iM3
SynthCommonSubExpressions(xH1::yM1
const;void
SetParams
cL1
x1&xL3
SetParamsMove(x1&tA1
CodeTree
GetUniqueRef();
#ifdef __GXX_EXPERIMENTAL_CXX0X__
void
SetParams(x1&&tA1
#endif
void
SetParam
iB1
which,lQ3
b);void
nG1
size_t
which,x82
b);void
AddParam
cL1
x82
param);void
AddParamMove(x82
param);void
AddParams
cL1
x1&xL3
AddParamsMove(x1&xL3
AddParamsMove(x1&lX2,size_t
lY2);void
DelParam
iB1
index);void
DelParams();void
Become
cL1
x82
b);inline
size_t
GetParamCount(yZ1
l02).xM3;xI1
x82
GetParam
iB1
n)yR
l02)[n];xI1
lQ3
GetParam
iB1
n
yZ1
l02)[n];xI1
void
nG2
x92
o)t13
Opcode=o;xI1
x92
GetOpcode()yQ
Opcode;xI1
nB
fphash_t
GetHash()yQ
Hash;xI1
const
x1&l02
yZ1
xY;xI1
x1&l02)yR
xY;xI1
size_t
y02
yQ
Depth;xI1
const
n82&GetImmed()yQ
Value;xI1
iG2
GetVar()yQ
l12
xI1
iG2
GetFuncNo()yQ
l12
xI1
bool
IsDefined(yZ1
GetOpcode()!=nB
cNop;xI1
bool
IsImmed(yZ1
GetOpcode()==nB
cImmed;xI1
bool
IsVar(yZ1
GetOpcode()==nB
l53;xI1
iG2
GetRefCount()yQ
xI3
c63
ReplaceWithImmed
cK1
i);void
Rehash(bool
constantfolding=true);void
Sort();inline
void
Mark_Incompletely_Hashed()t13
Depth=0;xI1
bool
Is_Incompletely_Hashed()yQ
Depth==0;xI1
const
tP
GetOptimizedUsing()yQ
l32;xI1
void
SetOptimizedUsing
cL1
tP
g)t13
l32=g;}
bool
RecreateInversionsAndNegations(bool
prefer_base2=false);void
iN3;void
swap(x82
b){data.swap(b.data);}
bool
IsIdenticalTo
cL1
x82
b
lB4;void
l61}
nJ1
cU2
xF2{int
xI3;x92
Opcode;n82
Value;iG2
l12
eH
Params;nB
fphash_t
Hash
iM3
Depth;const
tP
l32;xF2();xF2
cL1
xF2&b);i2
x92
o);i2
x92
o,iG2
f);i2
const
n82&i);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
i2
n82&&i);xF2(xF2&&b);
#endif
bool
IsIdenticalTo
cL1
xF2&b
lB4;void
Sort();void
Recalculate_Hash_NoRecursion();private:void
eA1=cL1
xF2&b);}
nJ1
yX
CodeTreeImmed
cK1
i)yR
eJ(i
yA
xK3());}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
cB1
CodeTreeImmed(n82&&i)yR
eJ
c42
i)yA
xK3());}
#endif
cB1
CodeTreeOp(x92
opcode)yR
eJ(opcode
yA
OpcodeTag());}
cB1
CodeTreeFuncOp(x92
t43
iG2
f)yR
eJ(t43
f
yA
FuncOpcodeTag());}
cB1
CodeTreeVar
nT2
varno)yR
eJ(varno
yA
VarTag());}
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
tY1
DumpHashes(xA)xY3
DumpTree(xA)xY3
DumpTreeWithIndent(xA,const
std
c23&indent="\\"
);
#endif
}
#endif
#endif
#ifndef FPOptimizer_GrammarHH
#define FPOptimizer_GrammarHH
#include <iostream>
t5{xR3
class
CodeTree;}
lP3
l21{enum
ImmedConstraint_Value{ValueMask=0x07,Value_AnyNum=0x0,nY2=0x1,Value_OddInt=0x2,tS1=0x3,Value_NonInteger=0x4,eJ1=0x5
c83
ImmedConstraint_Sign{SignMask=0x18,Sign_AnySign=0x00,nH1=0x08,eK1=0x10,Sign_NoIdea=0x18
c83
ImmedConstraint_Oneness{OnenessMask=0x60,Oneness_Any=0x00,Oneness_One=0x20,Oneness_NotOne=0x40
c83
ImmedConstraint_Constness{ConstnessMask=0x180,Constness_Any=0x00,tR1=0x80,Constness_NotConst=0x100
c83
Modulo_Mode{Modulo_None=0,Modulo_Radians=1
c83
Situation_Flags{LogicalContextOnly=0x01,NotForIntegers=0x02,OnlyForIntegers=0x04,OnlyForComplex=0x08,NotForComplex=0x10
c83
nR2{NumConstant,e93,SubFunction
c83
ParamMatchingType{PositionalParams,SelectedParams,AnyParams,GroupFunction
c83
RuleType{ProduceNewTree,ReplaceParams}
;
#ifdef __GNUC__
# define PACKED_GRAMMAR_ATTRIBUTE __attribute__((packed))
#else
# define PACKED_GRAMMAR_ATTRIBUTE
#endif
typedef
std::pair<nR2,const
void*>cX2
nJ1
cX2
e81
nT2
paramlist,nA1)nJ1
bool
ParamSpec_Compare
cL1
void*xJ3
void*b,nR2
type);iG2
ParamSpec_GetDepCode
cL1
cX2&b);cU2
ParamSpec_ParamHolder{nA1:8;iG2
constraints:9;iG2
depcode:15;}
e9
xR3
cU2
ParamSpec_NumConstant{n82
constvalue;iG2
modulo;}
;cU2
iZ{iG2
param_count:2;iG2
param_list:30;x92
subfunc_opcode:8;ParamMatchingType
match_type:3;iG2
n1:5;}
e9
cU2
ParamSpec_SubFunction{iZ
data;iG2
constraints:9;iG2
depcode:7;}
e9
cU2
Rule{RuleType
ruletype:2;iG2
situation_flags:5;iG2
repl_param_count:2+9;iG2
repl_param_list:30;iZ
match_tree;}
e9
cU2
Grammar{iG2
rule_count;iG2
short
rule_list[999
cS
extern
const
Rule
grammar_rules[];}
tY1
DumpParam
cL1
cX2&p,std::ostream&o=xT3)xY3
DumpParams
nT2
paramlist,iG2
count,std::ostream&o=xT3);}
#endif
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#define CONSTANT_POS_INF HUGE_VAL
#define CONSTANT_NEG_INF (-HUGE_VAL)
lP3
FUNCTIONPARSERTYPES{xR3
inline
n82
fp_const_pihalf()yR
fp_const_pi
xK()*n82(0.5);}
xR3
inline
n82
fp_const_twopi(){n82
xA3(fp_const_pi
xK());xA3
xQ
fp_const_twoe(){n82
xA3(fp_const_e
xK());xA3
xQ
fp_const_twoeinv(){n82
xA3(fp_const_einv
xK());xA3
xQ
fp_const_negativezero()yR-Epsilon
xK::value;}
}
#ifdef FP_SUPPORT_OPTIMIZER
#include <vector>
#include <utility>
#include <iostream>
yL1{using
lP3
l21;using
t5;using
lP3
FUNCTIONPARSERTYPES
nJ1
class
MatchInfo{t23
std::vector<std::pair<bool,eH> >lQ;eH
paramholder_matches;std::vector<iG2>tR;t23
MatchInfo():lQ(),paramholder_matches(),tR(){}
t23
bool
SaveOrTestRestHolder
nT2
n1,iI2&iF1){cV1{lQ.t33
n1+1);lQ
tQ=iF1
xH2
if(lQ[n1
tE3
lP4){lQ
tQ=iF1
xH2
iI2&found=lQ[n1
tF3;if(iF1.xM3!=found.xM3
cL
for
iB1
a=0;a<iF1.xM3;++a)if(!iF1[a]xF
found[a])cL
return
true
c63
SaveRestHolder
nT2
n1,eH&iF1){cV1
lQ.t33
n1+1);lQ
tQ.swap(iF1);}
bool
SaveOrTestParamHolder
nT2
nZ,lC2
xX3
eD3
cE
xM3<=nZ){cE
y13
nZ+1);cE
t33
nZ);cE
push_back(xX3)xH2
if(!cH.t91
cH=xX3
xH2
return
xX3
xF
cH)c63
SaveMatchedParamIndex(nA1){tR.push_back(index);}
lC2
GetParamHolderValueIfFound
nT2
nZ
lB4{static
const
eJ
dummytree;if(cE
xM3<=nZ)return
dummytree
n31
cH;}
lC2
GetParamHolderValue
nT2
nZ
yZ1
cH;}
bool
HasRestHolder
nT2
n1
yZ1
lQ.xM3>n1&&lQ[n1
tE3==true;}
iI2&GetRestHolderValues
nT2
n1
lB4{static
iI2
empty_result;cV1
return
empty_result
n31
lQ[n1
tF3;}
const
std::vector<iG2>&GetMatchedParamIndexes(yZ1
tR
c63
swap(tB
b){lQ.swap(b.lQ);cE
swap(b.paramholder_matches);tR.swap(b.tR);}
tB
eA1=cL1
tB
b){lQ=b.lQ;paramholder_matches=b.paramholder_matches;tR=b.tR
n31*this;}
}
;class
e2;typedef
xR<e2>cW;class
e2{t23
int
xI3;t23
e2():xI3(0){}
virtual~e2(){}
}
;cU2
lX1{bool
found;cW
specs;lX1(bool
f):found(f),specs(){}
lX1(bool
f,const
cW&s):found(f),specs(s){}
}
xY3
SynthesizeRule
cL1
eP2
eJ&tree
t53)nJ1
lX1
TestParam
cL1
cX2&yE2
xZ3,const
iO3)nJ1
lX1
TestParams(tC&nN,xZ3,const
iO3,bool
tW2
nJ1
bool
ApplyGrammar
cL1
Grammar&t52,FPoptimizer_CodeTree::eJ&tree,bool
from_logical_context=false)xY3
ApplyGrammars(FPoptimizer_CodeTree::cZ2
nJ1
bool
IsLogisticallyPlausibleParamsMatch(tC&c02,xZ3);}
lP3
l21{tY1
DumpMatch
cL1
eP2
nO,const
FPoptimizer_Optimize::tB
info,bool
DidMatch,std::ostream&o=xT3)xY3
DumpMatch
cL1
eP2
nO,const
FPoptimizer_Optimize::tB
info,xO3
tZ3,std::ostream&o=xT3);}
#endif
#include <string>
xP3
l21::nR2
yS1=false);xP3
x92
yS1=false);
#include <string>
#include <sstream>
#include <assert.h>
#include <iostream>
using
lP3
l21;using
lP3
FUNCTIONPARSERTYPES;xP3
l21::nR2
yS1){
#if 1
xO3
p=0;switch(opcode
iR2
t83
p="NumConstant"
;lC
e93:p="ParamHolder"
;lC
SubFunction:p="SubFunction"
tF2}
std::ostringstream
tmp;assert(p);tmp<<p;if(pad)while(tmp.str().xM3<12)tmp<<' '
n31
tmp.str();
#else
std::ostringstream
tmp;tmp<<opcode;if(pad)while(tmp.str().xM3<5)tmp<<' '
n31
tmp.str();
#endif
}
xP3
x92
yS1){
#if 1
xO3
p=0;switch(opcode
iR2
cAbs:p="cAbs"
;lC
cAcos:p="cAcos"
;lC
cAcosh:p="cAcosh"
;lC
cArg:p="cArg"
;lC
cAsin:p="cAsin"
;lC
cAsinh:p="cAsinh"
;lC
cAtan:p="cAtan"
;lC
cAtan2:p="cAtan2"
;lC
cAtanh:p="cAtanh"
;lC
cCbrt:p="cCbrt"
;lC
cCeil:p="cCeil"
;lC
cConj:p="cConj"
;lC
cCos:p="cCos"
;lC
cCosh:p="cCosh"
;lC
cCot:p="cCot"
;lC
cCsc:p="cCsc"
;lC
cExp:p="cExp"
;lC
cExp2:p="cExp2"
;lC
cFloor:p="cFloor"
;lC
cHypot:p="cHypot"
;lC
cIf:p="cIf"
;lC
cImag:p="cImag"
;lC
cInt:p="cInt"
;lC
cLog:p="cLog"
;lC
cLog2:p="cLog2"
;lC
cLog10:p="cLog10"
;lC
cMax:p="cMax"
;lC
cMin:p="cMin"
;lC
cPolar:p="cPolar"
;lC
cPow:p="cPow"
;lC
cReal:p="cReal"
;lC
cSec:p="cSec"
;lC
cSin:p="cSin"
;lC
cSinh:p="cSinh"
;lC
cSqrt:p="cSqrt"
;lC
cTan:p="cTan"
;lC
cTanh:p="cTanh"
;lC
cTrunc:p="cTrunc"
;lC
cImmed:p="cImmed"
;lC
cJump:p="cJump"
;lC
cNeg:p="cNeg"
;lC
cAdd:p="cAdd"
;lC
cSub:p="cSub"
;lC
cMul:p="cMul"
;lC
cDiv:p="cDiv"
;lC
cMod:p="cMod"
;lC
cEqual:p="cEqual"
;lC
cNEqual:p="cNEqual"
;lC
cLess:p="cLess"
;lC
cLessOrEq:p="cLessOrEq"
;lC
cGreater:p="cGreater"
;lC
cGreaterOrEq:p="cGreaterOrEq"
;lC
cNot:p="cNot"
;lC
cAnd:p="cAnd"
;lC
cOr:p="cOr"
;lC
cDeg:p="cDeg"
;lC
cRad:p="cRad"
;lC
cFCall:p="cFCall"
;lC
cPCall:p="cPCall"
tF2
#ifdef FP_SUPPORT_OPTIMIZER
case
cFetch:p="cFetch"
;lC
cPopNMov:p="cPopNMov"
;lC
eL3:p="cLog2by"
;lC
cNop:p="cNop"
tF2
#endif
case
cSinCos:p="cSinCos"
;lC
cSinhCosh:p="cSinhCosh"
;lC
e03:p="cAbsNot"
;lC
cAbsNotNot:p="cAbsNotNot"
;lC
cAbsAnd:p="cAbsAnd"
;lC
cAbsOr:p="cAbsOr"
;lC
cAbsIf:p="cAbsIf"
;lC
cDup:p="cDup"
;lC
cInv:p="cInv"
;lC
cSqr:p="cSqr"
;lC
cRDiv:p="cRDiv"
;lC
cRSub:p="cRSub"
;lC
cNotNot:p="cNotNot"
;lC
cRSqrt:p="cRSqrt"
;lC
l53:p="VarBegin"
tF2}
std::ostringstream
tmp;assert(p);tmp<<p;if(pad)while(tmp.str().xM3<12)tmp<<' '
n31
tmp.str();
#else
std::ostringstream
tmp;tmp<<opcode;if(pad)while(tmp.str().xM3<5)tmp<<' '
n31
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
;lP3
xH1{xR3
class
ByteCodeSynth{t23
ByteCodeSynth():ByteCode(),Immed(),cM(),xS(0),StackMax(0){i8
y13
64);Immed.y13
8);cM.y13
16)c63
Pull(std::vector<iG2>&bc,std::vector
xK&imm,size_t&StackTop_max){for(eQ1=0;a<i8
xM3;++a){l52]&=~nJ2;}
i8
swap(bc);Immed.swap(imm);StackTop_max=StackMax;}
size_t
GetByteCodeSize(yZ1
i8
xM3;}
size_t
GetStackTop(yZ1
xS
c63
PushVar
nT2
varno){xJ2
varno);eQ2}
void
PushImmed(n82
immed
nL
xJ2
cImmed);Immed.push_back(immed);eQ2}
void
StackTopIs(nO,int
offset=0
eD3(int)xS>offset){cM
l62
first=true;cM
l62
second=tree;}
}
bool
IsStackTop(nO,int
offset=0
yZ1(int)xS>offset&&cM
l62
first&&cM
l62
second
xF
tree);xI1
void
EatNParams
nT2
eat_count){xS-=eat_count
c63
ProducedNParams
nT2
produce_count){xJ1
xS+produce_count)c63
DoPopNMov
iB1
cY2,size_t
srcpos
nL
xJ2
cPopNMov)nS2
cY2)nS2
srcpos);xJ1
srcpos+1);cM[cY2]=cM[srcpos];xJ1
cY2+1)c63
DoDup
iB1
y23
nL
if(y23==xS-1){xJ2
cDup);}
else{xJ2
cFetch)nS2
y23);}
eQ2
cM[xS-1]=cM[y23];}
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
i21
int>void
Dump(){std::ostream&o=xT3;o<<"Stack state now("
<<xS<<"):\n"
cV3
0;a<xS;++a){o<<a<<": "
;if(cM[a
tE3){nO=cM[a
tF3;o<<'['<<std::hex<<(void*)(&tree.l02))<<std::dec<<','<<tree.GetRefCount()<<']'
e83
tree,o);}
else
o<<"?"
;o<<"\n"
;}
o<<std::flush;}
#endif
size_t
y33
nO
lB4{for
iB1
a=xS;a-->0;)if(cM[a
tE3&&cM[a
tF3
xF
tree
lB3
a
n31
eR2;}
bool
Find(nO
yZ1
y33
tree)!=eR2;}
bool
FindAndDup(nO){size_t
pos=y33
tree
e53
pos!=eR2){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<lE4"duplicate at ["
<<pos<<"]: "
e83
tree
n52" -- issuing cDup or cFetch\n"
;
#endif
DoDup(pos)xH2
return
tM3
cU2
IfData{size_t
ofs;}
;void
SynthIfStep1
l72,x92
op
x21
xI2=i8
xM3;xJ2
op
i9
xJ2
nJ2)c63
SynthIfStep2
l72
x21
iS
nU2)+2);iS
2
y7
l42
xI2=i8
xM3;xJ2
cJump
i9
xJ2
nJ2)c63
SynthIfStep3
l72
x21
i8
back()|=nJ2;iS
nU2)-1);iS
2
y7
l42
eQ2
for
iB1
a=0;a<xI2;++a
eD3
l52]==cJump&&l52+1]==(nJ2|(xI2-1))){l52+nU2)-1);l52+2
y7
l42
lG3
l52]iR2
cAbsIf:case
cIf:case
cJump:case
cPopNMov:a+=2;lC
cFCall:case
cPCall:case
cFetch:a+=1
tF2
c13
c43}
}
protected:void
xJ1
size_t
value){xS=value;if(xS>lR3{StackMax=xS;cM.t33
lR3;}
}
protected:std::vector<iG2>ByteCode;std::vector
xK
Immed;std::vector<std::pair<bool,FPoptimizer_CodeTree::eJ> >cM
iM3
xS
iM3
StackMax;private:void
incStackPtr(){if(xS+2>lR3
cM.t33
StackMax=xS+2);}
i21
bool
IsIntType,bool
IsComplexType>cU2
c12{}
;t23
void
AddOperation
nT2
t43
iG2
eat_count,iG2
produce_count=1){EatNParams(eat_count);lO1
opcode);ProducedNParams(produce_count)c63
lO1
iG2
t43
c12<false,false>yS
false,true>yS
true,false>yS
true,true>);inline
void
lO1
iG2
opcode){lO1
t43
c12<bool(nB
IsIntType
xK::xA3),bool(nB
IsComplexType
xK::xA3)>());}
}
nJ1
cU2
SequenceOpCode
nJ1
cU2
tN1{static
cI
AddSequence;static
cI
MulSequence;}
xY3
x01
long
count,cI&eU,yM1;}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3
xH1{xR3
cU2
SequenceOpCode{n82
basevalue;iG2
op_flip;iG2
op_normal,op_normal_flip;iG2
op_inverse,op_inverse_flip;}
nJ1
cI
tN1
xK::AddSequence={y21,cNeg
xE
cAdd,cSub,cRSub}
nJ1
cI
tN1
xK::MulSequence={n82(1),cInv,cMul,cMul,cDiv,cRDiv}
;
#define findName(a,b,c) "var"
#define TryCompilePowi(o) false
#define mData this
#define mByteCode ByteCode
#define mImmed Immed
xS3
false,false
l31
# define FP_FLOAT_VERSION 1
# define FP_COMPLEX_VERSION 0
# include "fp_opcode_add.inc"
# undef FP_COMPLEX_VERSION
# undef FP_FLOAT_VERSION
}
xS3
true,false
l31
# define FP_FLOAT_VERSION 0
# define FP_COMPLEX_VERSION 0
# include "fp_opcode_add.inc"
# undef FP_COMPLEX_VERSION
# undef FP_FLOAT_VERSION
}
#ifdef FP_SUPPORT_COMPLEX_NUMBERS
xS3
false,true
l31
# define FP_FLOAT_VERSION 1
# define FP_COMPLEX_VERSION 1
# include "fp_opcode_add.inc"
# undef FP_COMPLEX_VERSION
# undef FP_FLOAT_VERSION
}
xS3
true,true
l31
# define FP_FLOAT_VERSION 0
# define FP_COMPLEX_VERSION 1
# include "fp_opcode_add.inc"
# undef FP_COMPLEX_VERSION
# undef FP_FLOAT_VERSION
}
#endif
#undef findName
#undef mImmed
#undef mByteCode
#undef mData
#undef TryCompilePowi
}
using
lP3
xH1;
#define POWI_TABLE_SIZE 256
#define POWI_WINDOW_SIZE 3
lP3
xH1{
#ifndef FP_GENERATING_POWI_TABLE
extern
const
iG2
char
powi_table[POWI_TABLE_SIZE];const
#endif
iG2
char
powi_table[POWI_TABLE_SIZE]={0,1,1,1,2,1,2,1,y43
4,1,2,y53
2,1,y43
8,eP3
y63
15,1,16,1,2,1,4,1,2,y53
2,1,4,eP3
1,16,1,25,y63
27,5,8,3,2,1,30,1,31,3,32,1,2,1,y43
8,1,2,y63
39,1,16,137,2,1,4,eP3
y53
45,135,4,31,2,5,32,1,2,131,50,1,51,1,8,3,2,1,54,1,55,3,16,1,57,133,4,137,2,135,60,1,61,3,62,133,63,1,iG1
131,iG1
139,l82
e3
30,1,130,137,2,31,l82
e3
e3
130,eP3
1,e3
e3
2,1,130,133,iG1
61,130,133,62,139,130,137,e3
l82
e3
e3
iG1
131,e3
e3
130,131,2,133,l82
130,141,e3
130,eP3
1,e3
5,135,e3
l82
e3
l82
130,133,130,141,130,131,e3
e3
2,131}
;}
static
x63
yY=256;
#define FPO(x)
lP3{class
PowiCache{private:int
iB[yY];int
iH1[yY];t23
PowiCache():iB(),iH1(){iB[1]=1;}
bool
Plan_Add(c52,int
count){cC1>=yY
cL
iH1[eS2+=count
n31
iB[eS2!=0
c63
lS3
c52){cC1<yY)iB[eS2=1
c63
Start
iB1
value1_pos){for(int
n=2;n<yY;++n)iB[n]=-1;Remember(1,value1_pos);DumpContents();}
int
Find(c52
lB4{cC1<yY
eD3
iB[eS2>=0){FPO(iW3(l04,"* I found %ld from cache (%u,%d)\n",value,(unsigned)cache[value],iX3 value]))n31
iB[eS2;}
}
return-1
c63
Remember(c52,size_t
lC4){cC1>=yY)return;FPO(iW3(l04,"* Remembering that %ld can be found at %u (%d uses remain)\n",value,(unsigned)lC4,iX3 value]));iB[eS2=(int)lC4
c63
DumpContents
l94
FPO(for(int a=1;a<POWI_CACHE_SIZE;++a)if(cache[a]>=0||iX3 a]>0){iW3(l04,"== cache: sp=%d, val=%d, needs=%d\n",cache[a],a,iX3 a]);})}
int
UseGetNeeded(c52){cC1>=0&&value<yY)return--iH1[eS2
n31
0;}
}
nJ1
size_t
y8
long
count
lF2
cI&eU,yM1
xY3
c01
size_t
apos,long
aval,size_t
bpos,long
bval
lF2
iG2
cumulation_opcode,iG2
cimulation_opcode_flip,yM1;void
l41
c52
lF2
int
need_count,int
l51=0){cC1<1)return;
#ifdef FP_GENERATING_POWI_TABLE
if(l51>32)throw
false;
#endif
if(iB.Plan_Add(value,need_count
lB3;long
y83
1;cC1<POWI_TABLE_SIZE){y83
powi_table[eS2
xN3&128){half&=127
xN3&64)y83-tU2
FPO(iW3(l04,"value=%ld, half=%ld, otherhalf=%ld\n",value,half,value/half));l41
half
y73
iB.lS3
half)n31;}
lI2
half&64){y83-tU2}
}
else
cC1&1)y83
value&((1<<POWI_WINDOW_SIZE)-1);else
y83
value/2;long
cN=value-half
xN3>cN||half<0)std::swap(half,cN);FPO(iW3(l04,"value=%ld, half=%ld, otherhalf=%ld\n",value,half,otherhalf))xN3==cN){l41
half,iB,2,l51+1);}
else{l41
half
y73
l41
cN>0?cN:-cN
y73}
iB.lS3
value);}
xR3
size_t
y8
c52
lF2
cI&eU,yM1{int
y93=iB.Find(value
e53
y93>=0)yR
y93;}
long
y83
1;cC1<POWI_TABLE_SIZE){y83
powi_table[eS2
xN3&128){half&=127
xN3&64)y83-tU2
FPO(iW3(l04,"* I want %ld, my plan is %ld * %ld\n",value,half,value/half))iM3
xK2=y8
half
tW1
if(iB
lK2
half)>0||xK2!=tX1){iI1
xK2)e02
half,tX1);}
x01
value/half
tG2
size_t
lC4=tX1
e02
value,lC4);iB.DumpContents()n31
lC4;}
lI2
half&64){y83-tU2}
}
else
cC1&1)y83
value&((1<<POWI_WINDOW_SIZE)-1);else
y83
value/2;long
cN=value-half
xN3>cN||half<0)std::swap(half,cN);FPO(iW3(l04,"* I want %ld, my plan is %ld + %ld\n",value,half,value-half))xN3==cN){size_t
xK2=y8
half
tW1
c01
xK2,half,xK2,half,iB,eU.op_normal,eU.op_normal_flip,synth);}
else{long
part1=half;long
part2=cN>0?cN:-cN
iM3
part1_pos=y8
part1
tW1
size_t
part2_pos=y8
part2
tW1
FPO(iW3(l04,"Subdivide(%ld: %ld, %ld)\n",value,half,otherhalf));c01
part1_pos,part1,part2_pos,part2,iB,cN>0?eU.op_normal:eU.op_inverse,cN>0?eU.op_normal_flip:eU.op_inverse_flip,synth);}
size_t
lC4=tX1
e02
value,lC4);iB.DumpContents()n31
lC4;}
tY1
c01
size_t
apos,long
aval,size_t
bpos,long
bval
lF2
iG2
cumulation_opcode,iG2
cumulation_opcode_flip,yM1{int
a_needed=iB
lK2
aval);int
yA3=iB
lK2
bval);bool
lL2=false;
#define DUP_BOTH() do{if(apos<bpos){size_t tmp=apos;apos=bpos;bpos=tmp;lL2=!lL2;}FPO(iW3(l04,"-> " l64 l64"op\n",(unsigned)apos,(unsigned)bpos));iI1 apos);iI1 apos==bpos?tX1:bpos);}while(0)
#define DUP_ONE(p) do{FPO(iW3(l04,"-> " l64"op\n",(unsigned)p));iI1 p);}while(0)
if(a_needed>0
eD3
yA3>0){nK2}
e43
bpos!=tX1)nK2
else{lM2
lL2=!lL2;}
}
}
lI2
yA3>0
eD3
apos!=tX1)nK2
else
DUP_ONE(bpos);}
e43
apos==bpos&&apos==tX1)lM2
lI2
apos==tX1&&bpos==synth.xM
2){FPO(iW3(l04,"-> op\n"));lL2=!lL2;}
lI2
apos==synth.xM
2&&bpos==tX1)FPO(iW3(l04,"-> op\n"));lI2
apos==tX1)DUP_ONE(bpos);lI2
bpos==tX1){lM2
lL2=!lL2;}
else
nK2}
lT3
lL2?cumulation_opcode_flip:cumulation_opcode,2);}
tY1
cD1
long
count,cI&eU,yM1{while
eR3<256){int
y83
xH1::powi_table[count]xN3&128){half&=127;cD1
half
tG2
count/=half;}
else
c43
if
eR3==1)return;if(!eR3&1)){lT3
cSqr,1);cD1
count/2
tG2}
else{iI1
tX1);cD1
count-1
tG2
lT3
cMul,2);}
}
}
lP3
xH1{tY1
x01
long
count,cI&eU,yM1{if
eR3==0)lG2
eU.basevalue);else{bool
eT2=false;if
eR3<0){eT2=true;count=-count;}
if(false)cD1
count
tG2
lI2
count>1){PowiCache
iB;l41
count,iB,1)iM3
xK1=synth.GetStackTop();iB.Start(tX1);FPO(iW3(l04,"Calculating result for %ld...\n",count))iM3
xL2=y8
count
tW1
size_t
n_excess=synth.xM
xK1;if(n_excess>0||xL2!=xK1-1){synth.DoPopNMov(xK1-1,xL2);}
}
if(eT2)lT3
eU.op_flip,1);}
}
}
#endif
#ifndef FPOptimizer_ValueRangeHH
#define FPOptimizer_ValueRangeHH
t5{lP3
lU3{iM
cU2
Comp{}
;i21>cU2
Comp<nB
cLess>{i21
lE<n9
cLessOrEq>{i21
lE<=n9
cGreater>{i21
lE>n9
cGreaterOrEq>{i21
lE>=n9
cEqual>{i21
lE==n9
cNEqual>{i21
lE!=b;}
}
;}
xR3
cU2
cE1{n82
val;bool
known;cE1():val(),known(false){}
cE1
cK1
v):val(v),known(true){xI1
yB3
cK1
v){known=true;val=v;}
yB3(n82(iJ1(n82),cE1
y9)val=iJ2
yB3(n82(iJ1
cK1),cE1
y9)val=iJ2
iM
void
set_if(n82
v
tG1
iJ1(n82),cE1
y9&&lU3::Comp<Compare>()(val,v))val=iJ2
iM
void
set_if
cK1
v
tG1
iJ1
cK1),cE1
y9&&lU3::Comp<Compare>()(val,v))val=iJ2}
nJ1
cU2
range{cE1
xK
min,max;range():min(),max(){}
range(n82
mi,n82
ma):min(mi),max(ma){}
range(bool,n82
ma):min(),max(ma){}
range(n82
mi,bool):min(mi),max(){}
void
set_abs();void
set_neg();}
nJ1
bool
IsLogicalTrueValue
cL1
yV3&p
tY2
nJ1
bool
IsLogicalFalseValue
cL1
yV3&p
tY2;}
#endif
#ifndef FPOptimizer_RangeEstimationHH
#define FPOptimizer_RangeEstimationHH
t5{enum
TriTruthValue{IsAlways,IsNever,Unknown}
nJ1
yV3
iN
xZ3)nJ1
bool
IsLogicalValue
cL1
cZ2
nJ1
TriTruthValue
GetIntegerInfo
cL1
cZ2
nJ1
xL1
GetEvennessInfo
cL1
cZ2{if(!tree
i71)return
Unknown;yJ1=tree
nC1;if(nB
isEvenInteger(value
lC3
nB
isOddInteger(value
lD3
xR3
xL1
GetPositivityInfo
cL1
cZ2{yV3
p=iN
tree
e53
p
n51&&p
tQ2>=n82(lC3
p
yN
known
lH1
lD3
xR3
xL1
GetLogicalValue
lV3
tree
tY2{yV3
p=iN
tree
e53
IsLogicalTrueValue(p,abs
lC3
IsLogicalFalseValue(p,abs
lD3}
#endif
#ifndef FPOptimizer_ConstantFoldingHH
#define FPOptimizer_ConstantFoldingHH
t5{tY1
ConstantFolding(cZ2;}
#endif
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5;cU2
ComparisonSetBase{enum{t93=0x1,Eq_Mask=0x2,Le_Mask=0x3,tA3=0x4,tB3=0x5,Ge_Mask=0x6}
;static
int
Swap_Mask(int
m)yR(m&Eq_Mask)|((m&t93)?tA3:0)|((m&tA3)?t93:0);}
enum
c11{Ok,BecomeZero,BecomeOne,xN2
c83
nW2{cond_or,iK2,iL2,iM2}
;}
nJ1
cU2
ComparisonSet:public
ComparisonSetBase{cU2
eU2{eJ
a;eJ
b;int
relationship;eU2():a(),b(),relationship(){}
}
;std::vector<eU2>eK;cU2
Item{eJ
value;bool
c22;Item():value(),c22(false){}
}
;std::vector<Item>cF1;int
xM1;ComparisonSet():eK(),cF1(),xM1(0){}
c11
AddItem
lV3
a,bool
c22,nW2
type){for
iB1
c=0;c<cF1.xM3;++c)if(cF1[c].value
xF
a)eD3
c22!=cF1[c].c22){iQ
cW1
case
iM2:cF1.erase(cF1.begin()+c);xM1+=1
n31
xN2;case
iK2:case
iL2:cX1}
}
return
xN2;}
Item
pole;pole.value=a;pole.c22=c22;cF1.push_back(pole)n31
Ok;}
c11
AddRelationship(eJ
a,eJ
b,int
tO1,nW2
type){iQ
if(tO1==7)cW1
lC
iM2:if(tO1==7){xM1+=1
n21
lC
iK2:case
iL2:if(tO1==0)cX1
c43
if(!(a.GetHash()<b.GetHash())){a.swap(b);tO1=Swap_Mask(tO1);}
for
iB1
c=0;c<eK.xM3;++c
eD3
eK[c].a
xF
a)&&eK[c].b
xF
b)){iQ{int
yC3=xM2|tO1;if(yC3==7)cW1
xM2=yC3
tF2}
case
iK2:case
iL2:{int
yC3=xM2&tO1;if(yC3==0)cX1
xM2=yC3
tF2}
case
iM2:{int
newrel_or=xM2|tO1;int
xO2=xM2&tO1;lO2
5&&xO2==0){xM2=tB3
n21
lO2
7&&xO2==0){xM1+=1;eK.erase(eK.begin()+c)n21
lO2
7&&xO2==Eq_Mask){xM2=Eq_Mask;xM1+=1
n21
eM2}
return
xN2;}
}
eU2
comp;comp.a=a;comp.b=b;comp.relationship=tO1;eK.push_back(comp)n31
Ok;}
}
;nR1
n82,xG3
CondType>bool
ConstantFolding_LogicCommon(eJ&tree,CondType
xA1,bool
xP2){bool
should_regenerate=false;ComparisonSet
xK
comp;nB1{xG3
c6
c11
eS3=c6
Ok;lC2
atree=eT3;switch(atree
nC
iR2
cEqual
lG
Eq_Mask
iT2
cNEqual
lG
tB3
iT2
cLess
lG
t93
iT2
cLessOrEq
lG
Le_Mask
iT2
cGreater
lG
tA3
iT2
cGreaterOrEq
lG
Ge_Mask
iT2
cNot:eS3
yO1
l8
0),true
iT2
cNotNot:eS3
yO1
l8
0),false,xA1)tF2
c13
if(xP2||IsLogicalValue(atree))eS3
yO1,false,xA1);lG3
eS3){ReplaceTreeWithZero:xO
0)n31
true;ReplaceTreeWithOne:xO
1);nV
c6
Ok:lC
c6
BecomeZero
eL
c6
BecomeOne:i6
c6
xN2:c41
c43}
if(should_regenerate){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before ConstantFolding_LogicCommon: "
yA1
#endif
if(xP2){tree.DelParams();}
else{for
iY{lC2
atree=lE2
a
e53
IsLogicalValue(atree))eF2}
for
iB1
a=0;a<comp.cF1.xM3;++a
eD3
comp.cF1[a].c22
tH2
cNot);r.c5
r
tC3
lI2!xP2
tH2
cNotNot);r.c5
r
tC3
else
tree.c5}
for
iB1
a=0;a<comp.eK.xM3;++a
tH2
cNop);switch(comp.eK[a
eA
iR2
c6
t93:r
xD
cLess
lW3
Eq_Mask:r
xD
cEqual
lW3
tA3:r
xD
cGreater
lW3
Le_Mask:r
xD
cLessOrEq
lW3
tB3:r
xD
cNEqual
lW3
Ge_Mask:r
xD
cGreaterOrEq
tA2
r
yP1
comp.eK[a].a);r
yP1
comp.eK[a].b);r
tC3
if(comp.xM1!=0)tree.yC
n82(comp.xM1)));
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After ConstantFolding_LogicCommon: "
yA1
#endif
return
true;}
return
tM3
xN1
ConstantFolding_AndLogic(l14(tree.GetOpcode()==cAnd
iU3()==cAbsAnd)n31
nI
iK2,true);}
xN1
ConstantFolding_OrLogic(l14(tree.GetOpcode()==cOr
iU3()==cAbsOr)n31
nI
cond_or,true);}
xN1
ConstantFolding_AddLogicItems(l14(tree.GetOpcode()==cAdd)n31
nI
iM2,false);}
xN1
ConstantFolding_MulLogicItems(l14(tree.GetOpcode()==cMul)n31
nI
iL2,false);}
}
#include <vector>
#include <map>
#include <algorithm>
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5;cU2
CollectionSetBase{enum
xR1{Ok,xN2}
;}
nJ1
cU2
CollectionSet:public
CollectionSetBase{cU2
c21{eJ
value;eJ
xQ2;bool
cX;c21():value(),xQ2(),cX(false){}
c21
lV3
v,lC2
f):value(v),xQ2(f),cX(false){}
}
;std::multimap<fphash_t,c21>iC;typedef
xG3
std::multimap<fphash_t,c21>::yN3
xS1;CollectionSet():iC(){}
xS1
FindIdenticalValueTo
lV3
value){fphash_t
hash=value.GetHash();for(xS1
i=iC.xR2
hash);i!=iC.cY1
hash;++i){cC1
xF
i
e22.value
lB3
i;}
return
iC.end();}
bool
Found
cL1
xS1&b)yR
b!=iC.end();}
xR1
AddCollectionTo
lV3
xQ2,const
xS1&into_which){c21&c=into_which
e22;if(c.cX)c.xQ2
c9
xQ2);else{eJ
add;add
xD
cAdd);add
yP1
c.xQ2);add
c9
xQ2);c.xQ2.swap(add);c.cX=true;}
return
xN2;}
xR1
nX2
lV3
value,lC2
xQ2){const
fphash_t
hash=value.GetHash();xS1
i=iC.xR2
hash);for(;i!=iC.cY1
hash;++i
eD3
i
e22.value
xF
value
lB3
AddCollectionTo(xQ2,i);}
iC.yD3
i,std::make_pair(hash,c21(value,xQ2)))n31
Ok;}
xR1
nX2
lV3
a)yR
nX2(a,nF1
1)));}
}
nJ1
cU2
ConstantExponentCollection{typedef
eH
yF3;typedef
std::x31
xT2;std::vector<xT2>data;ConstantExponentCollection():data(){}
void
MoveToSet_Unique
cK1
eB1&eC1){data.push_back(std::x31(eB1()));data.back().second.swap(eC1)c63
MoveToSet_NonUnique
cK1
eB1&eC1){xG3
std::vector<xT2>::yN3
i=std::xR2
data.iN2
data.end(),exponent,Compare1st()e53
i!=data.cY1
xE2{i
e22.yD3
i
e22.end(),eC1.iN2
eC1.end());}
else{data.yD3
i,std::x31
tB2,eC1));}
}
bool
Optimize(){bool
changed=false;std::sort(data.iN2
data.end(),Compare1st());redo:for
iB1
a=0;a<data.xM3;++a){n82
exp_a=data[a
tE3
yI3
exp_a
tG1
1))cP2;for
iB1
b=a+1;b<data.xM3;++b){n82
exp_b=data[b
tE3;n82
xU2=exp_b-exp_a;if(xU2>=fp_abs(exp_a))break;n82
exp_diff_still_probable_integer=xU2*n82(16
e53
eV2
exp_diff_still_probable_integer)&&!(eV2
exp_b)&&!eV2
xU2))){yF3&a_set=lP2;yF3&b_set=data[b
tF3;
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before ConstantExponentCollection iteration:\n"
;eW2
cout);
#endif
if(isEvenInteger(exp_b)&&!isEvenInteger(xU2+exp_a)){eJ
tmp2;tmp2
eA3
tmp2.SetParamsMove(b_set);tmp2
eT1
t81
cAbs);tmp
yP1
tmp2
tH3
b_set.t33
1);b_set[0].i72}
a_set.yD3
a_set.end(),b_set.iN2
b_set.end());yF3
b_copy=b_set;data.erase(data.begin()+b);MoveToSet_NonUnique(xU2,b_copy);x42
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After ConstantExponentCollection iteration:\n"
;eW2
cout);
#endif
cZ}
}
}
return
changed;}
#ifdef DEBUG_SUBSTITUTIONS
void
eW2
ostream&out){for
iB1
a=0;a<data.xM3;++a){out.precision(12);out<<data[a
tE3<<": "
;iA1
lP2.xM3;++b
eD3
b>0)out<<'*'
e83
lP2[b],out);}
out<<xV3}
}
#endif
}
nJ1
static
eJ
x41
eJ&value,bool&xH){switch(value
nC
iR2
cPow:{eJ
e32
value
l8
1);value.y81
n31
exponent
iL3
cRSqrt:value.y81;xH=true
n31
nF1-0.5));case
cInv:value.y81;xH=true
n31
nF1-1));c13
c43
return
nF1
1));}
cA1
void
eD1
eV1&mul,xZ3,lC2
xQ2,bool&c31
bool&xH){nB1{eJ
value
y91
a));eJ
exponent(x41
value,xH)e53!xQ2
i71||xQ2
nC1!=n82(1.0)){eJ
cZ1;cZ1
eA3
cZ1
c9
xE2;cZ1
c9
xQ2);cZ1
eT1
exponent.swap(cZ1);}
#if 0 /* FIXME: This does not work */
cC1
nC==cMul
eD3
1){bool
exponent_is_even=exponent
i71&&isEvenInteger
tB2
nC1);iA1
value.tK3{bool
tmp=false;eJ
val(value
l8
b));eJ
exp(x41
val,tmp)e53
exponent_is_even||(exp
i71&&isEvenInteger(exp
nC1))){eJ
cZ1;cZ1
eA3
cZ1
c9
xE2;cZ1
yP1
exp);cZ1.ConstantFolding(e53!cZ1
i71||!isEvenInteger(cZ1
nC1)){goto
cannot_adopt_mul;}
}
}
}
eD1
mul,value,exponent,c31
xH);}
else
cannot_adopt_mul:
#endif
{if(mul.nX2(value,xE2==CollectionSetBase::xN2)c41}
}
}
xN1
ConstantFolding_MulGrouping(cZ2{bool
xH=false;bool
should_regenerate=false;eV1
mul;eD1
mul,tree,nF1
1)),c31
xH);typedef
std::pair<eJ,eH>eE1;typedef
std::multimap<fphash_t,eE1>cG1;cG1
tD;xV2
eV1::xS1
j=mul.iC.yG3
j!=mul.iC.end();++j){eJ&value=j
e22.value;eJ&e32
j
e22.xQ2;if(j
e22.cX)exponent
eT1
const
fphash_t
eF1=exponent.GetHash();xG3
cG1::yN3
i=tD.xR2
eF1);for(;i!=tD.cY1
eF1;++i)if(i
e22.first
xF
xE2
eD3!exponent
i71||!e01
nC1
tG1
1)))c41
i
e22.second.push_back(value);goto
skip_b;}
tD.yD3
i,std::make_pair(eF1,std::make_pair
tB2,eH
iB1(1),value))));skip_b:;}
#ifdef FP_MUL_COMBINE_EXPONENTS
ConstantExponentCollection
xK
e11;xV2
cG1::yN3
j,i=tD.yG3
i!=tD.end();i=j){j=i;++j;eE1&list=i
e22;if(list.first
l71
e32
list.first
nC1;if(!tB2==xD1)e11.MoveToSet_Unique
tB2,list
iO2;tD.erase(i);}
}
if(e11.Optimize())c41
#endif
if(should_regenerate){eJ
before=tree;before.l61
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before ConstantFolding_MulGrouping: "
e83
before
n52"\n"
;
#endif
tree.DelParams();xV2
cG1::yN3
i=tD.yG3
i!=tD.end();++i){eE1&list=i
e22;
#ifndef FP_MUL_COMBINE_EXPONENTS
if(list.first
l71
e32
list.first
nC1;if
tB2==xD1
continue;if(e01
x83
tree.AddParamsMove(list
iO2;eM2}
#endif
eJ
mul;mul
eA3
mul.SetParamsMove(list
iO2;mul
eT1
if(xH&&list.first
i61
list.first
nC1==n82(1)/n82(3)){eJ
cbrt;cbrt
xD
cCbrt);cbrt.e4
cbrt
cK
cbrt
xW
0.5)){eJ
sqrt;sqrt
xD
cSqrt);sqrt.e4
sqrt
cK
sqrt
xW-0.5)){eJ
rsqrt;rsqrt
xD
cRSqrt);rsqrt.e4
rsqrt
cK
rsqrt
xW-1)){eJ
inv;inv
xD
cInv);inv.e4
inv
cK
inv);eM2}
eJ
pow;pow
xD
cPow);pow.e4
pow
yP1
list.first);pow
cK
pow);}
#ifdef FP_MUL_COMBINE_EXPONENTS
tD.clear()cV3
0;a<i3.xM3;++a){n82
e32
i3[a
tE3;if(e01
x83
tree.AddParamsMove(i3[a]iO2;eM2
eJ
mul;mul
eA3
mul.SetParamsMove(i3[a]iO2;mul
eT1
eJ
pow;pow
xD
cPow);pow.e4
pow.yC
xE2);pow
cK
pow);}
#endif
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After ConstantFolding_MulGrouping: "
yA1
#endif
return!tree
xF
before);}
return
tM3
xN1
ConstantFolding_AddGrouping(cZ2{bool
should_regenerate=false;eV1
add;nB1{if
y91
a)nC==cMul
cP2;if(add.nX2
y91
a))==CollectionSetBase::xN2)c41}
cL3
iQ3(eM)iM3
tE=0;nB1{lC2
xF3=eT3;if
tD2
nC==cMul){iA1
yQ1
tK3{if
tD2
l8
b)i71
cP2;xG3
eV1::xS1
c=add.FindIdenticalValueTo
tD2
l8
b)e53
add.Found(c)){eJ
tmp
tD2
yA
CloneTag());tmp
tT2
b
tH3
add.AddCollectionTo(tmp,c);c41
goto
done_a;}
}
iQ3[a]=true;tE+=1;done_a:;}
}
if(tE>0
eD3
tE>1){std::vector<std::pair<eJ,iS3>nU;std::multimap<fphash_t,iS3
eG1;bool
lX3=false;nB1
iC1{iA1
eT3.tK3{lC2
p=eT3
l8
b);const
fphash_t
p_hash=p.GetHash();for(std::multimap<fphash_t,iS3::const_iterator
i=eG1.xR2
p_hash);i!=eG1.cY1
p_hash;++i
eD3
nU[i
e22
tE3
xF
p)){nU[i
e22
tF3+=1;lX3=true;goto
found_mulgroup_item_dup;}
}
nU.push_back(std::make_pair(p,size_t(1)));eG1.yD3
std::make_pair(p_hash,nU.xM3-1));found_mulgroup_item_dup:;}
}
if(lX3){eJ
e42;{size_t
max=0;for
iB1
p=0;p<nU.xM3;++p)if(nU[p
tF3<=1)nU[p
tF3=0;else{nU[p
tF3*=nU[p
tE3.y02;if(nU[p
tF3>max){e42=nU[p
tE3;max=nU[p
tF3;}
}
}
eJ
group_add;group_add
xD
cAdd);
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Duplicate across some trees: "
e83
e42
n52" in "
yA1
#endif
nB1
iC1
iA1
eT3.tK3
if(e42
xF
eT3
l8
b))){eJ
tmp
y91
a)yA
CloneTag());tmp
tT2
b
tH3
group_add
yP1
tmp);iQ3[a]=false
tF2}
group_add
eT1
eJ
group;group
eA3
group
yP1
e42);group
yP1
group_add);group
eT1
add.nX2(group);c41}
}
nB1
iC1{if(add.nX2
y91
a))==CollectionSetBase::xN2)c41}
}
if(should_regenerate){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before ConstantFolding_AddGrouping: "
yA1
#endif
tree.DelParams();xV2
eV1::xS1
j=add.iC.yG3
j!=add.iC.end();++j){eJ&value=j
e22.value;eJ&coeff=j
e22.xQ2;if(j
e22.cX)coeff
eT1
if(coeff
i61
yM3
coeff
nC1,xD1
cP2
yI3
coeff
nC1
x83
lP1
value);eM2}
eJ
mul;mul
eA3
mul
yP1
value);mul
yP1
coeff);mul
cK
mul);}
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After ConstantFolding_AddGrouping: "
yA1
#endif
return
true;}
return
tM3}
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5
nJ1
bool
ConstantFolding_IfOperations(l14(tree.GetOpcode()==cIf
iU3()==cAbsIf);for(;;){lY3
nC==cNot){eB3
cIf);lE2
0).xW2
0)lQ2
eW3.swap
y91
2));}
else
lY3
cS1{eB3
yO3;lE2
0).xW2
0)lQ2
eW3.swap
y91
2));}
else
break;lG3
tM
0),tZ1==yO3)l92
tree.xW2
1));nV
lL3
tree.xW2
2));nV
lT1
lY3
nC==cIf||lE2
0)nC==yO3{eJ
cond=lE2
0);eJ
n23;n23
e73==cIf?cNotNot
lI3
n23
xX2
1));ConstantFolding(n23);eJ
n33;n33
e73==cIf?cNotNot
lI3
n33
xX2
2));ConstantFolding(n33
e53
n23
i71||n33
i71){eJ
eV;eV
e73);eV
xX2
1));eV.nJ
1));eV.nJ
2));eV
eT1
eJ
eW;eW
e73);eW
xX2
2));eW.nJ
1));eW.nJ
2));eW
eT1
tree
e73);cH1
0,cond
lQ2
tree.nG1
1,eV);tree.nG1
2,eW)xH2}
if
y91
1)nC==lE2
2)nC&&y91
1)nC==cIf||eW3
nC==yO3){eJ&leaf1=eW3;eJ&leaf2=lE2
2);if
tE2
0)xC1
0))&&tE2
1)xC1
1))||leaf1
yP3
2)))){eJ
eV;eV
iQ2
eV.nJ
0));eV
iZ2
yQ3;eV
l03
yQ3;eV
eT1
eJ
eW;eW
iQ2
eW.nJ
0));eW
iZ2
yR3
eW
l03
yR3
eW
iV
SetParam(0
eV3
lQ2
tree.nG1
1,eV);tree.nG1
2,eW)xH2
if
tE2
1)xC1
1))&&leaf1
yP3
2))){eJ
eX;eX
iQ2
eX.AddParamMove
y91
0));eX
iZ2
lQ2
eX
l03
lQ2
eX
iV
nG1
0,eX);cH1
2
eV3
yR3
cH1
1
eV3
yQ3
xH2
if
tE2
1)xC1
2))&&leaf1
yP3
1))){eJ
e52;e52
xD
leaf2
nC==cIf?cNot:e03);e52
l03
lQ2
e52
eT1
eJ
eX;eX
iQ2
eX.AddParamMove
y91
0));eX
iZ2
lQ2
eX
yP1
e52);eX
iV
nG1
0,eX);cH1
2
eV3
yR3
cH1
1
eV3
yQ3
xH2}
eJ&xX=eW3;eJ&y6=lE2
2
e53
xX
xF
y6)){tree.xW2
1))xH2
const
OPCODE
op1=xX
nC;const
OPCODE
op2=y6
nC;if(op1==op2
eD3
xX
cI1
1){eJ
lO
0));n43
lQ2
tL3
n4
if(xX
cI1
2&&y6
cI1
2
eD3
xX
l8
0)xF
y6
l8
0))){eJ
param0=xX
l8
0);eJ
lO
1));n43
yQ3;tL3;lP1
param0)n4
if(xX
l8
1)xF
y6
yQ3){eJ
param1=xX
l8
1);eJ
lO
0));n43
lQ2
tL3;lP1
nE1);lP1
param1)xH2}
if(op1
yS3
cMul
lR2
cAnd
lR2
cOr
lR2
cAbsAnd
lR2
cAbsOr
lR2
cMin
lR2
cMax){eH
n53;yZ{for
iB1
b=y6.xF1
b-->0;){if
c32
y6
l8
b))eD3
n53
cT3){xX.l61
y6.l61}
n53.push_back(xX
nK3
y6
tT2
b);xX
t31
tA2}
}
if(!n53
cT3){xX
eT1
y6.Rehash()l7
op1);tree.SetParamsMove(n53)n4}
}
if(op1
yS3
cMul||(op1==cAnd
nT1
y6))||(op1==cOr
nT1
y6))){yZ
if
c32
y6)){xX.l61
xX
t31);xX
eT1
eJ
cO1=y6;y6=tF
op1
yS3
cOr)l13
op1);lP1
cO1)n4}
if((op1==cAnd
lR2
cOr)&&op2==cNotNot){eJ&n63=y6
l8
0);yZ
if
c32
n63)){xX.l61
xX
t31);xX
eT1
eJ
cO1=n63;y6=tF
op1
yH3
op1);lP1
cO1)n4}
if(op2==cAdd||op2==cMul||(op2==cAnd
nT1
xX))||(op2==cOr
nT1
xX))){for
iB1
a=y6
n41
y6
l8
a)xF
xX)){y6.l61
y6
t31);y6
eT1
eJ
cP1=xX;xX=tF
op2==cAdd||op2
yH3
op2);lP1
cP1)n4}
if((op2==cAnd||op2==cOr)&&op1==cNotNot){eJ&n73=xX
l8
0)cV3
y6
n41
y6
l8
a)xF
n73)){y6.l61
y6
t31);y6
eT1
eJ
cP1=n73;xX=tF
op2
yH3
op2);lP1
cP1)n4}
return
tM3}
#include <limits>
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5
nJ1
int
maxFPExponent()yR
std::numeric_limits
xK::max_exponent;}
xN1
x51
n82
base,n82
xE2{if(base<xD1
return
true
yI3
base,xD1||yM3
base
tG1
1))cL
return
exponent>=n82(maxFPExponent
xK())/fp_log2(base);}
xN1
ConstantFolding_PowOperations(l14(tree.GetOpcode()==cPow);nR&&eW3
l71
const_value
eF3
lR,tH);xO
const_value)n31
tM3
tR2
yM3
tH
x83
tree.xW2
0))xH2
nR&&yM3
lR
x83
xO
1)n31
tM3
nR&&eW3
nC==cMul){bool
xY2=false;n82
lS2=lR;eJ
xF3=eW3
cV3
xF3
n41
xF3
l8
a)l71
imm=xF3
l8
a)nC1;{if(x51
lS2,imm))break;n82
lT2
eF3
lS2,imm)yI3
lT2,xD1)break;if(!xY2){xY2=true;yQ1
l61}
lS2=lT2;yQ1
DelParam(a
tA2}
if(xY2){yQ1
Rehash();
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before pow-mul change: "
yA1
#endif
lE2
0).Become(e21
lS2));eW3.Become
tD2);
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After pow-mul change: "
yA1
#endif
}
}
tR2
lE2
0)nC==cMul){n82
lU2=tH;n82
xZ2=1.0;bool
xY2=false;eJ&xF3=lE2
0)cV3
xF3
n41
xF3
l8
a)l71
imm=xF3
l8
a)nC1;{if(x51
imm,lU2))break;n82
eH1
eF3
imm,lU2)yI3
eH1,xD1)break;if(!xY2){xY2=true;yQ1
l61}
xZ2*=eH1;yQ1
DelParam(a
tA2}
if(xY2){yQ1
Rehash();eJ
eX3;eX3
xD
cPow);eX3.SetParamsMove(tree.l02));eX3.n42
tree
eA3
lP1
eX3);tree
c9
e21
xZ2))xH2}
lY3
nC==cPow&&eW3
i71&&lE2
0)l8
1)l71
a=lE2
0)l8
1)nC1;n82
b=tH;n82
c=a*b;if(isEvenInteger(a)&&!isEvenInteger(c)){eJ
n83;n83
xD
cAbs);n83.nJ
0)lQ2
n83
eT1
tree.nG1
0,n83);}
else
cH1
0,lE2
0)lQ2
cH1
1,e21
c));}
return
tM3}
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5;cU2
l5{enum
e62{MakeFalse=0,MakeTrue=1,eX2=2,nA3=3,MakeNotNotP0=4,MakeNotNotP1=5,MakeNotP0=6,MakeNotP1=7,xB=8
c83
lV2{Never=0,Eq0=1,Eq1=2,yT3=3,yU3=4}
;e62
if_identical;e62
lW2
4];cU2{e62
what:4;lV2
when:4;}
iK1,iL1,iM1,iN1
nJ1
e62
Analyze
lV3
a,lC2
b
lB4{if(a
xF
b
lB3
if_identical;yV3
p0=iN
a);yV3
p1=iN
b);c93
known&&p1
n51){c93
val<p1
tQ2&&lW2
0]i4
0];c93
val<=p1
tQ2&&lW2
1]i4
1];}
if(p0
n51&&p1
xD2{if(p0
tQ2>p1
yN
val&&lW2
2]i4
2];if(p0
tQ2>=p1
yN
val&&lW2
3]i4
3];}
if(IsLogicalValue(a)eD3
iK1
tV2
iK1.when,p1
lB3
iK1.what;if(iM1
tV2
iM1.when,p1
lB3
iM1.what;}
if(IsLogicalValue(b)eD3
iL1
tV2
iL1.when,p0
lB3
iL1.what;if(iN1
tV2
iN1.when,p0
lB3
iN1.what;}
return
xB;}
cA1
bool
TestCase(lV2
when,const
yV3&p
eD3!p
n51||!p
yN
known
cL
switch(when
iR2
Eq0
nV1==n82(0.0)&&e23==p
tQ2;case
Eq1
nV1==n82(1.0)&&e23==e23;case
yT3
nV1>y21&&e23<=n82(1);case
yU3
nV1>=y21
lH1
1);c13;}
return
tM3}
;lP3
RangeComparisonsData{static
const
l5
Data[6]={{l5
c51,{l5::tU
xB,l5::tU
xB
iT
Eq1
iU
Eq1
nX1
Eq0
nY1
Eq0}
eI1
c51,l5::xB,l5
c51,l5::xB
iT
Eq0
iU
Eq0
nX1
Eq1
nY1
Eq1}
eI1
c51,l5::eX2,l5::tU
MakeFalse
nX1
yT3
iU
yU3
yD
cC
yD
n93{l5::xB,l5
c51,l5::tU
nA3
nX1
yU3
iU
yT3
yD
cC
eI1::tU
tU
n93
l5::eX2
iT
yU3
nY1
yT3
yD
cC
yD
n93{l5::tU
nA3,l5::xB,l5
c51
iT
yT3
nY1
yU3
yD
cC}
}
;}
xN1
ConstantFolding_Comparison(cZ2{using
lP3
RangeComparisonsData;assert(tree.GetOpcode()>=cEqual&&tree.GetOpcode()<=cGreaterOrEq);switch(Data[tZ1-cEqual].Analyze
y91
0),eW3)iR2
l5::MakeFalse:xO
0);nV
l5
c51:xO
1
iU2
nA3:eB3
cEqual
iU2
eX2:eB3
cNEqual
iU2
MakeNotNotP0:eB3
cNotNot
iV2
1
iU2
MakeNotNotP1:eB3
cNotNot
iV2
0
iU2
MakeNotP0:eB3
cNot
iV2
1
iU2
MakeNotP1:eB3
cNot
iV2
0
iU2
xB:;}
if
y91
1)i71)switch
y91
0)nC
iR2
cAsin:lM
fp_sin
yZ3
cAcos:lM
fp_cos
tG));eB3
tZ1==cLess?cGreater:tZ1==cLessOrEq?cGreaterOrEq:tZ1==cGreater?cLess:tZ1==cGreaterOrEq?cLessOrEq:tZ1);nV
cAtan:lM
fp_tan
yZ3
cLog:lM
fp_exp
yZ3
cSinh:lM
fp_asinh
yZ3
cTanh:if(fp_less(fp_abs
tG
x83
lM
fp_atanh
tG))xH2
break;c13
c43
return
tM3}
#include <list>
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3{
#ifdef DEBUG_SUBSTITUTIONS
yE
double
d)tN3
double
d;uint_least64_t
h;}
eY2
d=d;lM1
h
nZ1
#ifdef FP_SUPPORT_FLOAT_TYPE
yE
float
f)tN3
float
f;uint_least32_t
h;}
eY2
f=f;lM1
h
nZ1
#endif
#ifdef FP_SUPPORT_LONG_DOUBLE_TYPE
yE
long
double
ld)tN3
long
double
ld;cU2{uint_least64_t
a;iG2
short
b;}
s;}
eY2
ld=ld;lM1
s.b<<data.s.a
nZ1
#endif
#ifdef FP_SUPPORT_LONG_INT_TYPE
yE
long
ld){o<<"("
<<std::hex<<ld
nZ1
#endif
#endif
}
t5{lN
nE)){}
lN
const
n82&i
yA
xK3
nE
i)){data
x7
#ifdef __GXX_EXPERIMENTAL_CXX0X__
lN
n82&&i
yA
xK3
nE
std::move(i))){data
x7
#endif
lN
iG2
v
yA
VarTag
nE
l53,v
xS2
x92
o
yA
OpcodeTag
nE
o
xS2
x92
o,iG2
f
yA
FuncOpcodeTag
nE
o,f
xS2
lC2
b
yA
CloneTag
nE*b.data)){}
xR3
eJ::~CodeTree(){}
lB
ReplaceWithImmed
cK1
i){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Replacing "
e83*this
e53
IsImmed())OutFloatHex(xT3,GetImmed()n52" with const value "
<<i;OutFloatHex(xT3,i
n52"\n"
;
#endif
data=new
xF2
xK(i);}
xR3
cU2
ParamComparer{iH2()lV3
a,lC2
b
lB4{if(a.y02!=b.y02)return
a.y02<b.y02
n31
a.GetHash()<b.GetHash();}
}
xY3
xF2
xK::Sort(){switch(Opcode
iR2
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
cNEqual:std::sort(l33
iN2
l33
end(),ParamComparer
xK());lC
cLess
lY
cGreater;}
lC
cLessOrEq
lY
cGreaterOrEq;}
lC
cGreater
lY
cLess;}
lC
cGreaterOrEq
lY
cLessOrEq;}
break;c13
c43}
lB
AddParam
lV3
param){xY.push_back(param);}
lB
AddParamMove(eJ&param){xY.push_back(eJ());xY.back().swap(param);}
lB
SetParam
iB1
which,lC2
b)nW1
which
l23
xY[which]=b;}
lB
nG1
size_t
which,eJ&b)nW1
which
l23
xY[which
i13
b);}
lB
AddParams
cL1
nK){xY.yD3
xY.end(),lX2.iN2
lX2.end());}
lB
AddParamsMove(nK){size_t
endpos=xY.xM3,added=lX2.xM3;xY.t33
endpos+added,eJ());for
iB1
p=0;p<added;++p)xY[endpos+p
i13
lX2[p]);}
lB
AddParamsMove(nK,size_t
lY2)nW1
lY2
l23
DelParam(lY2);AddParamsMove(tA1}
lB
SetParams
cL1
nK){eH
tmp(tA1
xY.i72}
lB
SetParamsMove(nK){xY.swap(tA1
lX2.clear();}
#ifdef __GXX_EXPERIMENTAL_CXX0X__
lB
SetParams(eH&&lX2){SetParamsMove(tA1}
#endif
lB
DelParam
iB1
index){eH&Params=xY;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
l33
erase(l33
begin()+index);
#else
Params[index].data=0;for
iB1
p=index;p+1<l33
xM3;++p)Params[p].data.UnsafeSetP(&*Params[p+1
l23
Params[l33
xM3-1].data.UnsafeSetP(0);l33
t33
l33
xM3-1);
#endif
}
lB
DelParams(){xY.clear();}
xN1
eJ::IsIdenticalTo
lV3
b
lB4{if(&*data==&*b.data)return
true
n31
data->IsIdenticalTo(*b.data);}
xN1
xF2
xK::IsIdenticalTo
cL1
xF2
xK&b
lB4{if(Hash!=b.Hash
cL
if(Opcode!=i23
cL
switch(Opcode
iR2
cImmed:return
yM3
Value,i43;case
l53:return
l22==b.l12
case
cFCall:case
cPCall:if(l22!=b.l22
cL
break;c13
c43
if(l33
xM3!=b.l33
xM3
cL
for
iB1
a=0;a<l33
xM3;++a
eD3!Params[a]xF
b.Params[a])cL}
return
true;}
lB
Become
lV3
b
eD3&b!=this&&&*data!=&*b.data){DataP
tmp=b.data;l61
data.i72}
}
lB
CopyOnWrite(eD3
GetRefCount()>1)data=new
xF2
xK(*data);}
xR3
eJ
eJ::GetUniqueRef(eD3
GetRefCount()>1)return
eJ(*this,CloneTag())n31*this;}
tX):yT
cNop),Value(),n8
tX
const
xF2&b):yT
i23),Value(i43,l22(b.cQ1,Params(b.Params),Hash(b.Hash),Depth(b.Depth),tP1
b.l32){}
tX
const
n82&i):yT
cImmed),Value(i),n8
#ifdef __GXX_EXPERIMENTAL_CXX0X__
tX
xF2
xK&&b):yT
i23),Value
c42
i43),l22(b.cQ1,Params
c42
b.Params)),Hash(b.Hash),Depth(b.Depth),tP1
b.l32){}
tX
n82&&i):yT
cImmed),Value
c42
i)),n8
#endif
tX
x92
o):yT
o),Value(),n8
tX
x92
o,iG2
f):yT
o),Value(),l22(f),Params(),Hash(),Depth(1),tP1
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
lP3
FUNCTIONPARSERTYPES;
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
lP3{tY1
lZ2(nO,std
c7&done,std::ostream&o){nB1
lZ2
tO3
done,o);std::ostringstream
buf
e83
tree,buf);done[y03].yD3
buf.str());}
}
#endif
t5{
#ifdef FUNCTIONPARSER_SUPPORT_DEBUGGING
tY1
DumpHashes(cF){std
c7
done;lZ2(tree,done,o);for(std
c7::const_iterator
i=done.yG3
i!=done.end();++i){const
std::set<std
c23>&flist=i
e22;if(flist.xM3!=1)o<<"ERROR - HASH COLLISION?\n"
;for(std::set<std
c23>::const_iterator
j=flist.yG3
j!=flist.end();++j){o<<'['<<std::hex<<i->first.hash1<<','<<i->first.hash2<<']'<<std::dec;o<<": "
<<*j<<"\n"
;}
}
}
tY1
DumpTree(cF){xO3
iZ3;switch(tZ1
iR2
cImmed:o<<tree
nC1
c03
l53:o<<"Var"
<<(tree.GetVar()-l53)c03
cAdd:iZ3"+"
;lC
cMul:iZ3"*"
;lC
cAnd:iZ3"&"
;lC
cOr:iZ3"|"
;lC
cPow:iZ3"^"
tF2
c13
iZ3;o<<eE3
tZ1);lH3
cFCall||tZ1==cPCall)o<<':'<<tree.GetFuncNo();}
o<<'(';if(eM<=1&&sep2[1])o<<(sep2+1)<<' ';nB1{if(a>0)o<<' ';DumpTree
tO3
o
e53
a+1<eM)o<<sep2;}
o<<')';}
tY1
DumpTreeWithIndent(cF,const
std
c23&indent){o<<'['<<std::hex<<(void*)(&tree.l02))<<std::dec<<','<<tree.GetRefCount()<<']';o<<indent<<'_';switch(tZ1
iR2
cImmed:o<<"cImmed "
<<tree
nC1;o<<'\n'
c03
l53:o<<"VarBegin "
<<(tree.GetVar()-l53);o<<'\n'
n31;c13
o<<eE3
tZ1);lH3
cFCall||tZ1==cPCall)o<<':'<<tree.GetFuncNo();o<<'\n';}
nB1{std
c23
ind=indent;for
iB1
p=0;p<ind.xM3;p+=2)if(ind[p]=='\\')ind[p]=' ';ind+=(a+1<eM)?" |"
:" \\"
;DumpTreeWithIndent
tO3
o,ind);}
o<<std::flush;}
#endif
}
#endif
using
lP3
l21;using
lP3
FUNCTIONPARSERTYPES;
#include <cctype>
lP3
l21{iG2
ParamSpec_GetDepCode
cL1
cX2&b){switch(b.first
iR2
e93:{cO*s=(cO*)b.second
n31
s->depcode
iL3
SubFunction:{cP*s=(cP*)b.second
n31
s->depcode;}
c13
c43
return
0;}
tY1
DumpParam
cL1
cX2&yE2
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
;iG2
y12
0;t73
t83{const
ParamSpec_NumConstant
xK
yL3
cL1
ParamSpec_NumConstant
xK*c61
using
lP3
FUNCTIONPARSERTYPES;o.precision(12);o<<l84
constvalue
tF2}
case
e93:{cO
yL3(cO*c61
o<<ParamHolderNames[l84
index];y12
l84
constraints
tF2}
case
SubFunction:{cP
lM4
y12
l84
constraints;yF
GroupFunction
eD3
l84
l81==cNeg){o<<"-"
;n2}
lI2
l84
l81==cInv){o<<"/"
;n2}
else{std
c23
opcode=eE3(x92)l84
l81).substr(1)cV3
0;a<opcode.xM3;++a)opcode[a]=(char)std::toupper(opcode[a]);o<<opcode<<"( "
;n2
o<<" )"
;}
}
else{o<<'('<<eE3(x92)l84
l81)<<' ';yF
PositionalParams)o<<'[';yF
SelectedParams)o<<'{';n2
if
cU3
n1!=0)o<<" <"
<<l74.n1<<'>';yF
PositionalParams)o<<"]"
;yF
SelectedParams)o<<"}"
;o<<')';}
c43
lG3
ImmedConstraint_Value
yK3
ValueMask)iR2
ValueMask:lC
Value_AnyNum:lC
nY2:o<<"@E"
;lC
Value_OddInt:o<<"@O"
;lC
tS1:o<<"@I"
;lC
Value_NonInteger:o<<"@F"
;lC
eJ1:o<<"@L"
tF2
lG3
ImmedConstraint_Sign
yK3
SignMask)iR2
SignMask:lC
Sign_AnySign:lC
nH1:o<<"@P"
;lC
eK1:o<<"@N"
tF2
lG3
ImmedConstraint_Oneness
yK3
OnenessMask)iR2
OnenessMask:lC
Oneness_Any:lC
Oneness_One:o<<"@1"
;lC
Oneness_NotOne:o<<"@M"
tF2
lG3
ImmedConstraint_Constness
yK3
ConstnessMask)iR2
ConstnessMask:lC
tR1:if(yF2.first==e93){cO
yL3(cO*c61
if(l84
index<2)c43
o<<"@C"
;lC
Constness_NotConst:o<<"@V"
;lC
Oneness_Any:c43}
tY1
DumpParams
nT2
paramlist,iG2
count,std::ostream&o){for(eQ1=0;a<count;++a
eD3
a>0)o<<' ';const
cX2&param=e81
xK(paramlist,a);DumpParam
xK(param,o);iG2
depcode=ParamSpec_GetDepCode(param
e53
depcode!=0)o<<"@D"
<<depcode;}
}
}
#include <algorithm>
using
lP3
l21;using
lP3
FUNCTIONPARSERTYPES;lP3{cO
plist_p[37]={{2,0,0x0}
nP
0,0x4}
nP
nH1,0x0}
nP
eK1|Constness_NotConst,0x0}
nP
Sign_NoIdea,0x0}
nP
eJ1,0x0}
,{3,Sign_NoIdea,0x0}
,{3,0,0x0}
,{3,eJ1,0x0}
,{3,0,0x8}
,{3,Value_OddInt,0x0}
,{3,Value_NonInteger,0x0}
,{3,nY2,0x0}
,{3,nH1,0x0}
,{0,eK1|lV{0,lV{0,nH1|lV{0,nY2|lV{0,tR1,0x1}
,{0,tS1|nH1|lV{0,tT1
tR1,0x1}
,{0,tT1
lV{0,Oneness_One|lV{0,eJ1|lV{1,lV{1,nY2|lV{1,tT1
lV{1,tS1|lV{1,nH1|lV{1,eK1|lV{6,0,0x0}
,{4,0,0x0}
,{4,tS1,0x0}
,{4,lV{4
eG3
5,0,0x0}
,{5,lV}
nJ1
cU2
plist_n_container{static
const
ParamSpec_NumConstant
xK
plist_n[20];}
nJ1
const
ParamSpec_NumConstant
xK
plist_n_container
xK::plist_n[20]={{n82(-2
tY-1
tY-0.5
tY-0.25
tY
0
eZ2
fp_const_deg_to_rad
xK(eZ2
fp_const_einv
xK(eZ2
fp_const_log10inv
xK(tY
0.5
eZ2
fp_const_log2
xK(tY
1
eZ2
fp_const_log2inv
xK(tY
2
eZ2
fp_const_log10
xK(eZ2
fp_const_e
xK(eZ2
fp_const_rad_to_deg
xK(eZ2-fp_const_pihalf
xK(),xT1{y21,xT1{fp_const_pihalf
xK(),xT1{fp_const_pi
xK(),xT1}
;cP
plist_s[517]={{{1,15,t12
398,t12
477,t12
15,cNeg,GroupFunction,0}
,tR1,0x1}
,{{1,15,y22
24,y22
465,y22
466,y22
498,cInv,lT
2,327995
xE
l0
2,48276
xE
l6
260151
xE
l6
470171
xE
l6
169126
xE
l6
48418
xE
l6
1328
xE
l6
283962
xE
l6
169275
xE
l6
39202
xE
l6
283964
xE
l6
283973
xE
l6
476619
xE
l6
296998
xE
l6
47
xE
SelectedParams,0}
,0,0x4
nH
161839
xE
l6
25036
xE
l6
35847
xE
l6
60440
xE
l6
30751
xE
l6
270599
xE
l6
60431
xE
l6
259119
xE
l6
183474
xE
l6
332066
xE
l6
7168
xE
l6
197632
xE
l6
291840
xE
l6
283648
xE
l6
238866
xE
l6
239902
xE
l6
31751
xE
l6
244743
xE
l6
384022
xE
SelectedParams,0}
,0,0x4
nH
385262
xE
l6
386086
xE
l6
393254
xE
SelectedParams,0}
,0,0x5
nH
393254
xE
l6
386095
xE
l6
387312
xE
l6
18662
xE
l6
61670
xE
l6
387397
xE
l6
247855
xE
SelectedParams,0}
,0,0x1
nH
342063
xE
l6
297007
xE
l6
15820
xE
l6
393263
xE
l6
393263
xE
SelectedParams,0}
,0,0x5
nH
161847
xE
l6
258103
xE
l6
249073
xE
l6
249076
xE
iD
0,0
xE
nF
0,0
tU1
1,45
xE
nF
1,53
xE
nF
1,54
xE
nF
1,55
xE
nF
1,56
xE
nF
1,26
xE
nF
1,259
tJ
1}
eG3{1,253
xE
nF
1,272
tU1
1,323
tJ
1}
eG3{1,0
xE
nF
1,21
xE
nF
1,447
tJ
1}
lA2
449
tJ
1}
lA2
0
tJ
1}
lA2
0
tJ
2}
lA2
15
xE
nF
1,24
tJ
2}
,0,0x0
nH
58392
tU1
0,0
tJ
1}
,nH1,0x0
nH
24591
xU3
33807
xU3
48143
xU3
285720
xU3
290840
xU3
305152,l9
312400,l9
39202,l9
121894,l9
421926,l9
429094,l9
443430,l9
317834,l9
329098,l9
7633,l9
7706,l9
7730,l9
38,l9
50587,l9
406528,l9
24583,l9
31751,l9
405511,l9
321551,xU1
327713,l9
322596,l9
88361,l9
335174,l9
327050,l9
493606,l9
496678,l9
503846,l9
516134,l9
7217,l9
333875,l9
336896,l9
524326,l9
509952,l9
286727,l9
90127,l9
131087,l9
296976,tB1
324623,l1
0x14
nH
332815,l1
0x10}
,{{3,7340056,tB1
289092,l9
92176,xU1
337935
lB2
7340060,l1
t32
7340176,l9
338959
lB2
7340061,xU1
7206,l9
xE3
l9
357414,l9
368678,l9
370745,l1
0x7}
,{{3,7340177,l9
39277,tB1
426398,l1
t32
40272286,xU1
490910,l1
t32
40336798,xU1
50600,l9
426462,xU1
490974,xU1
370726,l1
0x6
nH
371750,l1
0x6
nH
428070
lB2
40336862,xU1
38378,l9
50671
lB2
47662080,l9
477184,l9
568320,l9
371727,l1
0x7}
,{{3,15779306,l9
370703,l1
0x7
nH
39277,l9
39279,l1
0x4}
,{{3,15779238,l9
39338,tB1
436262,l9
508966,l9
39409,tB1
296998,tB1
35847,l9
15,tB1
377894,l9
386063,l1
0x1
nH
15,l9
7192,l9
122904,l9
121880,l9
30751,l9
57,l9
7456,l9
15674
lB2
67579935,l9
39237,l9
58768,l9
62924,l9
121856,l9
15760
lB2
64009216,l1
0x0}
,{{0,0,xG
0,0,iL
2,e31
2,e41
3,e31
3,e41
38,xG
1,38,iL
14,xG
1,57,xG
1,16,e72
0x0
nH
471103,e72
0x1}
,{{1,303,xG
1,323,cA3
0x0
nH
471363,e72
0x16}
,{{1,293,e31
294,e41
295,xG
1,296,iL
400,xG
1,0,xG
1,460,xG
1,465,xG
1,16,e72
0x1}
,{{1,57,cA3
0x1}
,{{1,0,iL
21,xG
1,15,e72
0x0
nH
24591,xG
1,24,iL
517,cA3
0x0
nH
46095,lK
46104,lK
15397,lK
287789,lK
66584,lK
404763,lK
62504,lK
15409,lK
39951,lK
24591,lK
33807,lK
50200,lK
62509,lK
50176,lF,178176,eL1
0x12
nH
283648,lF,19456,lF,27648,lF,89088,lF,86016,lF,488448,lF,14342,lF,58375,lF,46147
xZ
46151,lF,284679,lF,7183,lF,46159
xZ
38993
xZ
50265,lF,50249,lF,283808,lF,284835,lF,24822,lF,10240,lF,11264,lF,7170,lF,xE3
lF,17408,lF,164864,lF,237568,lF,242688,eL1
0x14
nH
476160,lF,25607,lF,121871,lF,50252,lF,39374,lF,50183,lF,7192,lF,121887,lF,252979,lF,46155,lF,38919,lF,50267,lF,50268,lF,50253,lF,46190,lF,50295,lF,7563,eL1
0x10
nH
416811,lF,416819,lF,40046,lF,46191
xZ
415795,lF,40047
xZ
415787,lF,39015,eL1
0x5
nH
39326
xZ
39326,lF,39332,eL1
0x5
nH
39333
e12
50590
xZ
50590,lF,39338
xZ
39338,lF,39335,eL1
0x5
nH
15786
xZ
146858,lF,39372,lF,39379,lF,39380,lF,39390
xZ
50654
xZ
50654,lF,24,eL1
0x6
nH
62,lF,24,lF,62,eL1
0x6
nH
43,lF,43
xZ
51,lF,51
xZ
50269,lF,50176
xZ
50270,lF,39159,lF,39183
xZ
7168
xZ
31744,lF,99328,lF,31746,lF,100376,lF,39409
xZ
39411
xZ
39411,lF,39420,lF,39420
xZ
15,lF,39025,eL1
0x5
nH
39422,lF,16384,lF,62853,lF,15360,lF,15
e12
16,lF,7183
e12
7172
xQ3
yX1}
,nH1,0x0
nH
24591
xQ3
lT
2,50200
xQ3
lT
2,63521
xQ3
lT
2,62500
xQ3
lT
2,50453
xQ3
lT
2,62488
xQ3
lT
1,0,tP3
7,tP3
194,tP3
0,cAcos
tQ3
cAcosh
tQ3
cAsin
tQ3
cAsinh
nQ
119,cAsinh
tQ3
cAtan
eW1
306176,cAtan2
eW1
xE3
cAtan2
tQ3
cAtanh
nQ
246,cCeil
tQ3
cCeil,l63
0,c62
0,cCos,l63
7,c62
91,c62
92,c62
119,c62
236,c62
255,c62
214,l73
236,l73
464,l73
0,cCosh,l63
0,l73
0,cExp
nQ
7,cExp
nQ
91,cExp
tQ3
cB3
7,cB3
91,cB3
246,cFloor
tQ3
cFloor,lA
0x4
nH
309540,cHypot
eW1
316708,cHypot
eW1
316724,cHypot,l0
3,32513024,y32
34627584
n61
31493120,y32
89213952
n61
149042176
n61
246647808
n61
301234176
n61
494360576
n61
498558976
n61
62933520
n61
62933520,y32
62933526
n61
62933526,y32
24670208
n61
579378176
n61
573578240
n61
32513024
n61
566254592
n61
7900160
n61
588822528,cIf
nQ
119,cInt
nQ
246
c72
0
c72
7
c72
31
c72
194
c72
363
c72
15,cLog,lT
1,24,cLog,lT
1,0,cLog10
tQ3
cLog2
eW1
xE3
cMax
eW1
35847,cMax
eW1
30751,cMax
tQ3
cMax,AnyParams,1}
,0,0x4
nH
xE3
cMin
eW1
35847,cMin
eW1
30751,cMin
tQ3
cMin,AnyParams,1}
,0,0x4
nH
24591,cMin,lT
1,0,nZ2
7,nZ2
91,nZ2
92,nZ2
119,nZ2
149,nZ2
231,cSin,lA
0x5}
,{{1,246,nZ2
255,nZ2
254,nZ2
0,cSin,l63
273,cSin,lA
0x1}
,{{1,214,y42
231,cSinh,lA
0x5}
,{{1,246,y42
254,y42
255,y42
464,y42
0,cSinh,l63
0,y42
15,cSqrt,lT
1,0,c82
0,cTan,l63
115,cTan,l63
116,c82
231,c82
246,c82
273,c82
254,c82
255,c82
0,y72
0,cTanh,l63
213,y72
231,y72
246,y72
254,y72
255,y72
0,cTrunc
eW1
15384,cSub,lT
2,15384,cDiv,lT
2,476626,cDiv,lT
2,121913
t02
xE3
n22
tR3
nH
7168
t02
31744,n22
lA
0x20
nH
31751,n22
lA
0x24
nH
31751
t02
121913,eM1
l0
2,xE3
cLess,tR3
nH
41984,cLess,lA
0x4
nH
41984,cLess
eW1
7,cLess
eW1
xE3
cLessOrEq
eW1
296182,cLessOrEq
eW1
xE3
cGreater,tR3
nH
41984,cGreater,lA
0x4
nH
41984,cGreater
eW1
7,cGreater
eW1
xE3
yB
l0
2,296182,cGreaterOrEq
tQ3
n02
245,n02
7,n02
550,n02
553,n02
554,n02
556,n02
31,n02
559,n02
15,n02
560,cNot
eW1
7706,nB3
xE3
nB3
35847,nB3
30751,nB3
463903,nB3
466975,cAnd,iD
0,0,cAnd,nF
2,xE3
eY3
7706,eY3
35847,eY3
463903,eY3
466975,eY3
30751,cOr,iD
1,0,n12
91,n12
131,n12
245,n12
215,n12
246,cDeg
nQ
246,cRad
eW1
xE3
cAbsAnd,l6
xE3
cAbsOr,iD
1,0,e03
tQ3
cAbsNotNot,l0
3,32513024,cJ3
lA
0x0}
,}
;}
lP3
l21{const
Rule
grammar_rules[262]={{ProduceNewTree,17,1,0,{1,0,cAbs,e82
409,{1,146,cAtan,e82
403
nP
1324,cC3
e82
405
nP
307201,cC3
l2
yZ2
253174
nP
255224,cC3
l2
yZ2
259324
nP
257274,cC3
e82
152,{1,252,cCeil
iO
486,{1,68,t42
482,{1,122,t42
483,{1,124,t42
151,{1,125,t42
419,{1,123,t42
0,{1,403,cCos,l2
2,1,246,{1,252,cCos,l2
18,1,0,{1,400,t42
301,{1,406,cCosh,l2
2,1,246,{1,252,cCosh,l2
18,1,0,{1,400,cCosh
iO
458,{1,121,cFloor,e82
150,{1,252,cFloor,tS3
156,{3,7382016,eB
549,{3,8430592,eB
556,{3,8436736,eB
157,{3,42998784,eB
550,{3,42999808,eB
562,{3,43039744,eB
557,{3,49291264,eB
538,{3,49325056,eB
469,{3,1058318,eB
473,{3,1058324,eB
473,{3,9438734,eB
469,{3,9438740,cIf,l2
0,3,32542225,{3,36732434,cIf,l2
0,3,32542231,{3,36732440
lD4
573,{3,32513026
lD4
515,{3,455505423
lD4
515,{3,433506837,cIf
iO
78,{1,256,tT3
69,{1,258,tT3
404,{1,72,tT3
159,{1,147,cLog,l2
0,1,0
nP
487425,cMax
l3
16,1,445
nP
cD3
cMax
l3
0,1,0
nP
483329,cMin
l3
16,1,446
nP
cD3
cMin,c0
0,1,153
nP
24832
xQ3
tS3
153
nP
25854
xQ3
tS3
154
nP
129039
xQ3
xV1
32055
xQ3
xV1
32056
xQ3
xV1
32057
xQ3
l2
0,2,166288
nP
32137
xQ3
xV1
33082
xQ3
l2
0,2,7168
nP
12688
xQ3
l2
0,2,7434
nP
12553
y52
435
nP
46146
y52
436
nP
46154
y52
437
nP
46150
y52
169
nP
83983
y52
168
nP
130082
y52
175
nP
133154
y62
476160
nP
471055
y62
274432
nP
273423
y62
251904
nP
266274
y62
251904
nP
263186
y52
171,{1,252,c92
421,{1,68,c92
151,{1,122,c92
419,{1,124,c92
170,{1,125,c92
482,{1,123,c92
0,{1,405,c92
172,{1,252,cSinh
iO
328,{1,404,cSinh
iO
173,{1,252,lG4
0,{1,408,lG4
176,{1,410,lG4
177,{1,252,cTanh,l2
0,1,442
nP
449551,tZ
1,441
nP
cD3
tZ
1,167
nP
268549,tZ
1,181
nP
276749,tZ
1,180
nP
276500,tZ
2,190770
nP
189622,tZ
2,194748
nP
193723,tZ
2,202943
nP
196795,tZ
2,59699
nP
298148,tZ
2,59714
nP
325815,tZ
2,59724
nP
343224
xE
c0
2,1,337,{1,333
tJ
1
l83
336,{1,338
tJ
1}
}
,{ReplaceParams,2,1,340
nP
1363,i0
342
nP
1365,i0
463
nP
472524,i0
47
nP
356711,i0
349
nP
200751,i0
360
nP
199727,i0
480
nP
207053,i0
481
nP
208077,i0
417
nP
211144,i0
209
nP
211145,i0
418
nP
215240,i0
212
nP
212329,i0
204
nP
373097,i0
211
nP
372944,i0
217
nP
201944,i0
221
nP
223448,i0
367
nP
508329,i0
219
nP
508126,i0
224
nP
225705,i0
223
nP
225776,i0
365
nP
230825,i0
426
nP
377057,i0
497
nP
377054,i0
497
nP
204201,i0
426
nP
375280,i0
224
nP
375006,cAdd
l3
2,2,407781
nP
233698,cAdd
l3
2,2,59763
nP
233842,tZ
1,372
nP
1397,lQ1
95
nP
24705,lQ1
96
nP
24708,lQ1
444
nP
449551,lQ1
443
nP
cD3
lQ1
100
nP
101750,lQ1
108
nP
106821,lQ1
105
nP
103749
nB2
0,2,110607
nP
108869
nB2
0,2,107535
nP
109893,lJ
0
l83
112
nP
111634,cMul,SelectedParams,0
l83
567,{1,52,lJ
1
l83
568,{1,42,lJ
1}
}
,{ReplaceParams,2,1,467
nP
45516
iW
356
nP
51555
iW
468
nP
49612
iW
357
nP
47459
iW
429
nP
438699
iW
432
nP
441774
iW
486
nP
498726
iW
494
nP
504870
iW
382
nP
435579
iW
497
nP
435709
iW
426
nP
508287
iW
414
nP
500092
iW
499
nP
352744
iW
345
nP
367092
iW
381
nP
425318
iW
478
nP
425460
iW
47
nP
512501
iW
505
nP
355817
iW
47
nP
516598
iW
507
nP
518182
iW
508
nP
358896
iW
351
nP
388605
iW
511
nP
360939
iW
503
nP
354788
iW
514
nP
525350
iW
510
nP
394342
iW
386
nP
351346
nB2
2,2,363004
nP
361968
nB2
16,1,117
nP
1157
nB2
16,1,118
nP
1158
nB2
16,1,402
nP
411024
nB2
16,2,58768
nP
1472
nB2
16,2,15760
nP
1474
nB2
17,1,0,{1,400
nB2
17,1,57,{1,14,lJ
0
l91
4,1,538
nP
41,n22
cE3
0
nP
5167,n22
y82
41984
nP
409641,cEqual
c4
n22
y82
eY
n22
l93
n22
l2
xB1
24849,n22
l2
xW3
n22
l2
yZ2
nC3
281873,n22
l2
yZ2
lA1,n22
l2
yZ2
lB1
n22
cE3
562
nP
41,eM1
cE3
538
nP
5167,eM1
y82
41984
nP
409641,cNEqual
c4
eM1
y82
eY
eM1
l93
eM1
l2
xB1
24849,eM1
l2
xW3
eM1
l2
yZ2
nC3
281873,eM1
l2
yZ2
lA1,eM1
l2
yZ2
lB1
cNEqual
c4
eO1
16,2,eY
eO1
16
eI
cLess,e82
571
nP
46080,eO1
xB1
24832,eO1
yZ2
yT1,eO1
xW3
eO1
cG3
eO1
yZ2
xW1,eO1
yZ2
lA1,eO1
yZ2
lB1
cLess,cF3
562
nP
409641,cLess
c4
t6
16,2,eY
t6
16
eI
cLessOrEq,e82
565
nP
409615,t6
xB1
24832,t6
yZ2
yT1,t6
xW3
t6
cG3
t6
yZ2
xW1,t6
yZ2
lA1,t6
yZ2
lB1
cLessOrEq,cF3
562
nP
409647,cLessOrEq
c4
cGreater,y82
eY
cGreater,l93
cGreater,e82
539
nP
409615
eP
529654
nP
24832
eP
yT1
eP
iE
eP
nC3
281856
eP
xW1
eP
lA1
eP
lB1
cGreater,cF3
538
nP
409647,cGreater
c4
yB
y82
eY
yB
l93
yB
e82
572
nP
46080,yB
l2
xB1
24832,yB
l2
yZ2
yT1,yB
l2
xW3
yB
l2
cG3
yB
l2
yZ2
xW1,yB
l2
yZ2
lA1,yB
l2
yZ2
lB1
yB
cF3
538
nP
409641,yB
cE3
519,{1,137,cNot,cH3
571,{1,2,cNot,l2
0,1,452
nP
cD3
nD3
0,2,537097,{3,547892744,cAnd,c0
16,1,566,{1,5,cAnd,AnyParams,1}
}
,{ReplaceParams,16,1,569
nP
13314,nD3
16,1,544
nP
553498,nD3
16,1,546
nP
462369,nD3
16,1,548
nP
466465,nD3
0,1,457
nP
cD3
xX1
570
nP
13314,xX1
563
nP
8197,xX1
541
nP
553498,xX1
542
nP
462369,xX1
543
nP
466465,xX1
564
nP
143365,cOr,c0
4,1,525,{1,137,cI3
cH3
572,{1,2,cI3
l4
17,1,0,{1,0,cI3
e82
537,{1,256,cAbsNotNot,c0
18,1,531,{1,254,cAbsNotNot,c0
0,1,572,{3,43039744,cJ3
tS3
571,{3,49325056,cJ3
cH3
454,{3,32513586,cJ3
l2
16,3,32542225,{3,36732434,cJ3
yX1}
}
,}
;cU2
grammar_optimize_abslogical_type{y3
9
cS
grammar_optimize_abslogical_type
grammar_optimize_abslogical={9,{34,192,228,238,242,247,254,260,261}
}
;}
cU2
grammar_optimize_ignore_if_sideeffects_type{y3
59
cS
grammar_optimize_ignore_if_sideeffects_type
grammar_optimize_ignore_if_sideeffects={59,{0,20,21,22,23,24,25,26,cQ
iO1
78,cR
cT
cU2
grammar_optimize_nonshortcut_logical_evaluation_type{y3
56
cS
grammar_optimize_nonshortcut_logical_evaluation_type
grammar_optimize_nonshortcut_logical_evaluation={56,{0,25,cQ
iO1
78,cR
241,243,244,245,246,248,249,250,251,252,253,255,256,257,258,259}
}
;}
cU2
grammar_optimize_recreate_type{y3
22
cS
grammar_optimize_recreate_type
grammar_optimize_recreate={22,{18,55,56,57,80,81,82,83,84,85,117,118,120,121,130,131,132,133,134,135,136,137}
}
;}
cU2
grammar_optimize_round1_type{y3
125
cS
grammar_optimize_round1_type
grammar_optimize_round1={125,{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,19,25,cQ
37,38,iO1
45,46,47,48,49,50,51,52,53,54,58,59,60,61,62,63,64,65,66,67,68,69,70,71,78,79,80,81,82,83,84,85,86,87,88,93,94,95,96,97,98,99,100,101,117,118,119,120,121,122,123,124,125,126,127,128,129,138,160,161,162,163,164,165,166,167,168,169,178,179,180,200,204,212,216,224,236,237,239,240,cT
cU2
grammar_optimize_round2_type{y3
103
cS
grammar_optimize_round2_type
grammar_optimize_round2={103,{0,15,16,17,25,cQ
39,40,iO1
45,46,47,48,49,50,51,52,53,54,59,60,72,73,78,79,86,87,88,89,90,91,92,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,119,122,123,124,125,126,127,128,139,159,160,161,162,163,164,165,166,167,168,169,178,179,180,200,204,212,216,224,236,237,239,240,cT
cU2
grammar_optimize_round3_type{y3
79
cS
grammar_optimize_round3_type
grammar_optimize_round3={79,{74,75,76,77,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,170,171,172,173,174,175,176,177,181,182,183,184,185,186,187,188,189,190,191,193,194,195,196,197,198,199,201,202,203,205,206,207,208,209,210,211,213,214,215,217,218,219,220,221,222,223,225,226,227,229,230,231,232,233,234,235}
}
;}
cU2
grammar_optimize_round4_type{y3
12
cS
grammar_optimize_round4_type
grammar_optimize_round4={12,{18,55,56,57,130,131,132,133,134,135,136,137}
}
;}
cU2
grammar_optimize_shortcut_logical_evaluation_type{y3
53
cS
grammar_optimize_shortcut_logical_evaluation_type
grammar_optimize_shortcut_logical_evaluation={53,{0,25,cQ
iO1
78,cR
cT}
lP3
l21{xR3
cX2
e81
nT2
paramlist,nA1){index=(paramlist>>(index*10))&1023;if(index>=57)return
cX2(SubFunction,cA2
plist_s[index-57]e53
index>=37)return
cX2(NumConstant,cA2
plist_n_container
xK::plist_n[index-37])n31
cX2(e93,cA2
plist_p[index]);}
}
#ifdef FP_SUPPORT_OPTIMIZER
#include <stdio.h>
#include <algorithm>
#include <map>
#include <sstream>
using
lP3
FUNCTIONPARSERTYPES;using
lP3
l21;using
t5;using
yL1;lP3{nR1
It,xG3
T,xG3
Comp>eP1
MyEqualRange(It
first,It
last,const
T&val,Comp
comp){size_t
len=last-first;while(len>0){size_t
y83
len/2;It
xB3(first);xB3+=half;if(comp(*xB3,val)){first=xB3;++first;len=len-half-1;}
lI2
comp(val,*xB3)){len=half;}
else{It
left(first);{It&e92=left;It
last2(xB3)iM3
len2=last2-e92;while(len2>0){size_t
half2=len2/2;It
c33(e92);c33+=half2;if(comp(*c33,val)){e92=c33;++e92;len2=len2-half2-1;}
else
len2=half2;}
}
first+=len;It
right(++xB3);{It&e92=right;It&last2=first
iM3
len2=last2-e92;while(len2>0){size_t
half2=len2/2;It
c33(e92);c33+=half2;if(comp(val,*c33))len2=half2;else{e92=c33;++e92;len2=len2-half2-1;}
}
}
return
eP1(left,right);}
}
return
eP1(first,first);}
xR3
cU2
OpcodeRuleCompare{iH2()lV3
tree,iG2
y92
lB4{const
Rule&rule=grammar_rules[y92]n31
tZ1<rule
cC2.subfunc_opcode;}
iH2()nT2
y92,xZ3
lB4{const
Rule&rule=grammar_rules[y92]n31
rule
cC2.subfunc_opcode<tZ1;}
}
nJ1
bool
TestRuleAndApplyIfMatch
cL1
eP2
eJ&tree,bool
c1{MatchInfo
xK
info;lX1
found(false,cW()e53(rule.lC1
LogicalContextOnly)&&!c1{tF1
if(nB
IsIntType
xK::xA3
eD3
rule.lC1
NotForIntegers)tF1
e43
rule.lC1
OnlyForIntegers)tF1
if(nB
IsComplexType
xK::xA3
eD3
rule.lC1
NotForComplex)tF1
e43
rule.lC1
OnlyForComplex)tF1
for(;;){
#ifdef DEBUG_SUBSTITUTIONS
#endif
found=TestParams(rule
cC2,tree,found.specs,info,true
e53
found.found)break;if(!&*found.specs){fail:;
#ifdef DEBUG_SUBSTITUTIONS
DumpMatch(rule
eQ3,false);
#endif
return
tM3}
#ifdef DEBUG_SUBSTITUTIONS
DumpMatch(rule
eQ3,true);
#endif
SynthesizeRule(rule
eQ3)xH2}
yL1{xN1
ApplyGrammar
cL1
Grammar&t52,eJ&tree,bool
c1{if(tree.GetOptimizedUsing()==&t52){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Already optimized:  "
e83
tree
n52"\n"
<<std::flush;
#endif
return
tM3
if(true){bool
changed=false;switch(tZ1
iR2
cNot:case
cNotNot:case
cAnd:case
cOr:for
iB1
a=0;a<tree.x6
true))x42
lC
cIf:case
cAbsIf:if(ApplyGrammar(t52,lE2
0),tZ1==cIf))x42
for
iB1
a=1;a<tree.x6
c1)x42
break;c13
for
iB1
a=0;a<tree.x6
false))x42}
if(changed){tree.Mark_Incompletely_Hashed()xH2}
typedef
const
iG2
short*nE3;std::pair<nE3,nE3>range=MyEqualRange(t52.rule_list,t52.rule_list+t52.rule_count,tree,OpcodeRuleCompare
xK());std::vector<iG2
short>rules;rules.y13
range.second-range.first);for
y0
if(IsLogisticallyPlausibleParamsMatch(tC1
cC2,tree))rules.push_back(*r);}
range.first=!rules
cT3?&rules[0]:0;range.second=!rules
cT3?&rules[rules.xM3-1]+1:0;if(range.first!=range
iO2{
#ifdef DEBUG_SUBSTITUTIONS
if(range.first!=range
iO2
yA2"Input ("
<<eE3
tZ1)<<")["
<<eM<<"]"
;if(c1
xT3<<"(Logical)"
;iG2
first=iP1,prev=iP1;xO3
sep=", rules "
;for
y0
if(first==iP1)first=prev=*r;lI2*r==prev+1)prev=*r;else
yA2
sep<<first;sep=","
;if(prev!=first)xT3<<'-'<<prev;first=prev=*r;}
}
if(first!=iP1)yA2
sep<<first;if(prev!=first)xT3<<'-'<<prev;}
xT3<<": "
e83
tree
n52"\n"
<<std::flush;}
#endif
bool
changed=false;for
y0
#ifndef DEBUG_SUBSTITUTIONS
if(!IsLogisticallyPlausibleParamsMatch(tC1
cC2,tree)cP2;
#endif
if(TestRuleAndApplyIfMatch(tC1,tree,c1){x42
c43}
if(changed){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Changed."
<<std::endl
iS2"Output: "
e83
tree
n52"\n"
<<std::flush;
#endif
tree.Mark_Incompletely_Hashed()xH2}
tree.SetOptimizedUsing(&t52)n31
tM3
xN1
ApplyGrammar
cL1
void*p,FPoptimizer_CodeTree::cZ2
yR
ApplyGrammar(*cL1
Grammar*)p,tree);}
tY1
ApplyGrammars(FPoptimizer_CodeTree::cZ2{
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_round1\n"
;
#endif
n6
grammar_optimize_round1
t22
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_round2\n"
;
#endif
n6
grammar_optimize_round2
t22
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_round3\n"
;
#endif
n6
grammar_optimize_round3
t22
#ifndef FP_ENABLE_SHORTCUT_LOGICAL_EVALUATION
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_nonshortcut_logical_evaluation\n"
;
#endif
n6
grammar_optimize_nonshortcut_logical_evaluation
t22
#endif
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_round4\n"
;
#endif
n6
grammar_optimize_round4
t22
#ifdef FP_ENABLE_SHORTCUT_LOGICAL_EVALUATION
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_shortcut_logical_evaluation\n"
;
#endif
n6
grammar_optimize_shortcut_logical_evaluation
t22
#endif
#ifdef FP_ENABLE_IGNORE_IF_SIDEEFFECTS
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_ignore_if_sideeffects\n"
;
#endif
n6
grammar_optimize_ignore_if_sideeffects
t22
#endif
#ifdef DEBUG_SUBSTITUTIONS
xT3<<iV3"grammar_optimize_abslogical\n"
;
#endif
n6
grammar_optimize_abslogical
t22
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
lP3
FUNCTIONPARSERTYPES;using
lP3
l21;using
t5;using
yL1;lP3{xN1
TestImmedConstraints
nT2
bitmask,xZ3){switch(bitmask&ValueMask
iR2
Value_AnyNum:case
ValueMask:lC
nY2:if(GetEvennessInfo
tG3
n32
Value_OddInt:if(GetEvennessInfo
tG3
x02
tS1:if(GetIntegerInfo
tG3
n32
Value_NonInteger:if(GetIntegerInfo
tG3
x02
eJ1:if(!IsLogicalValue(tree)cL
nI1
SignMask
iR2
Sign_AnySign:lC
nH1:if(l01
n32
eK1:if(l01
x02
Sign_NoIdea:if(l01
Unknown
cL
nI1
OnenessMask
iR2
Oneness_Any:case
OnenessMask:lC
Oneness_One:tU3
if(!yM3
fp_abs(tree
nC1)tG1
1))cL
lC
Oneness_NotOne:tU3
if(yM3
fp_abs(tree
nC1)tG1
1))cL
nI1
ConstnessMask
iR2
Constness_Any:lC
tR1:tU3
lC
Constness_NotConst:if(yN1
c43
return
true;}
i21
iG2
extent,iG2
nbits,xG3
eA2=iG2
int>cU2
nbitmap{private:static
const
iG2
bits_in_char=8;static
const
iG2
eB2=(cK3
eA2)*bits_in_char)/nbits;eA2
data[(extent+eB2-1)/eB2];t23
void
inc(nA1,int
by=1){data[pos(index)]+=by*eA2(1<<yB2);xI1
void
dec(nA1){inc(index,-1);}
int
get(nA1
yZ1(data[pos(index)]>>yB2)&mask();}
static
yU1
pos(nA1)yR
index/eB2;}
static
yU1
shift(nA1)yR
nbits*(index%eB2);}
static
yU1
mask()yR(1<<nbits)-1;}
static
yU1
mask(nA1)yR
mask()<<yB2;}
}
;cU2
eO3{int
SubTrees:8;int
Others:8;int
yC2:8;int
Immeds:8;nbitmap<l53,2>SubTreesDetail;eO3(){std::memset(this,0,cK3*this));}
eO3
cL1
eO3&b){std::memcpy(this,&b,cK3
b));}
eO3&eA1=cL1
eO3&b){std::memcpy(this,&b,cK3
b))n31*this;}
}
nJ1
eO3
CreateNeedList_uncached(tC&c02){eO3
cN1;for(eQ1=0;a<c02
yD2;++a){const
cX2&yF2=e81
xK(c02.param_list,a);t73
SubFunction:{cP
lM4
yF
GroupFunction)++t72;else{++t92;assert(l74.subfunc_opcode<VarBegin);cN1.SubTreesDetail.inc(l84
l81);}
++cN1.yC2
tF2}
case
t83
case
e93:++t82;++cN1.yC2
tF2}
}
return
cN1;}
xR3
eO3&CreateNeedList(tC&c02){typedef
std::map<tC*,eO3>e51;static
e51
yV1;e51::yN3
i=yV1.xR2&c02
e53
i!=yV1.cY1&c02)return
i
e22
n31
yV1.yD3
i,std::make_pair(&c02,CreateNeedList_uncached
xK(c02)))e22;}
xR3
eJ
lK4
cL1
cX2&yE2
const
tB
info){t73
t83{const
ParamSpec_NumConstant
xK
yL3
cL1
ParamSpec_NumConstant
xK*)yF2.second
n31
CodeTreeImmed(l84
constvalue)iL3
e93:{cO
yL3(cO*)yF2.second
n31
t63
GetParamHolderValueIfFound(l84
index)iL3
SubFunction:{cP
lM4
eJ
xA3;xA3
xD
l84
l81);x53
l02).y13
l74
yD2);for(eQ1=0;a<l74
yD2;++a){eJ
tmp(lK4(e81
xK
cU3
param_list,a),info));xA3
yP1
tmp);}
x53
Rehash()n31
xA3;}
}
return
eJ();}
}
yL1{xN1
IsLogisticallyPlausibleParamsMatch(tC&c02,xZ3){eO3
cN1(CreateNeedList
xK(c02))iM3
tV3=c8
if(tV3<size_t(cN1.yC2))t62
for
iB1
a=0;a<tV3;++a){iG2
opcode=eT3
nC;switch(opcode
iR2
cImmed:if(t72>0)--t72;else--t82;lC
l53:case
cFCall:case
cPCall:--t82
tF2
c13
assert(opcode<VarBegin);if(t92>0&&cN1.SubTreesDetail.get(opcode)>0){--t92;cN1.SubTreesDetail.dec(opcode);}
else--t82;}
}
if(t72>0||t92>0||t82>0)t62
if(c02.match_type!=AnyParams
eD3
0||t92<0||t82<0)t62}
return
true;}
xR3
lX1
TestParam
cL1
cX2&yE2
xZ3,const
iO3){t73
t83{const
ParamSpec_NumConstant
xK
yL3
cL1
ParamSpec_NumConstant
xK*c61
tU3
n82
imm=tree
nC1;switch(l84
modulo
iR2
Modulo_None:lC
Modulo_Radians:imm=fp_mod(imm,yO
imm<xD1
imm
yP
if(imm>fp_const_pi
xK())imm-=fp_const_twopi
xK(tA2
return
yM3
imm,l84
constvalue)iL3
e93:{cO
yL3(cO*c61
if(!x0
return
t63
SaveOrTestParamHolder(l84
index,tree)iL3
SubFunction:{cP
lM4
yF
GroupFunction
eD3!x0
eJ
xY1=lK4(yE2
info);
#ifdef DEBUG_SUBSTITUTIONS
DumpHashes(xY1
n52*cL1
void**)&xY1
nC1
iS2"\n"
iS2*cL1
void**)&tree
nC1
iS2"\n"
;DumpHashes(tree
n52"Comparing "
e83
xY1
n52" and "
e83
tree
n52": "
iS2(xY1
xF
tree)?"true"
:"false"
n52"\n"
;
#endif
return
xY1
xF
tree);}
e43!&eC2
eD3!x0
if(tZ1!=l84
l81
cL}
return
TestParams(l74,tree,start_at,info,false);}
}
}
return
tM3
xR3
cU2
iX
x52
MatchInfo
xK
info;iX()iP3,info(){}
}
nJ1
class
MatchPositionSpec_PositionalParams:xE1
iX
xK>{t23
lE3
MatchPositionSpec_PositionalParams
iB1
n
yB1
iX
xK>(n){}
}
;cU2
iQ1
x52
iQ1()iP3{}
}
;class
yG:xE1
iQ1>{t23
iG2
trypos;lE3
yG
iB1
n
yB1
iQ1>(n),trypos(0){}
}
nJ1
lX1
TestParam_AnyWhere
cL1
cX2&yE2
xZ3,const
iO3,cL3&used,bool
tW2{xR<yG>x5;tK2
yG
tW3
a=x5->trypos;goto
retry_anywhere_2;}
tP2
yG(eM);a=0;}
for(;a<c8++a
eD3
used[a]cP2;retry_anywhere
cM3
TestParam(yE2
eT3,(iW2)tX2
used[a]=true
iP
a);x5->trypos=a
n31
lX1(true,&*x5);}
}
retry_anywhere_2:tV1
goto
retry_anywhere;}
}
return
tM3
xR3
cU2
yC1
x52
MatchInfo
xK
info;cL3
used;lE3
yC1
iB1
tV3)iP3,info(),used(tV3){}
}
nJ1
class
MatchPositionSpec_AnyParams:xE1
yC1
xK>{t23
lE3
MatchPositionSpec_AnyParams
iB1
n,size_t
m
yB1
yC1
xK>(n,yC1
xK(m)){}
}
nJ1
lX1
TestParams(tC&nN,xZ3,const
iO3,bool
tW2{if(nN.match_type!=AnyParams
eD3
xV!=eM
cL}
if(!IsLogisticallyPlausibleParamsMatch(nN,tree))t62
switch(nN.match_type
iR2
PositionalParams:{xR<cG>x5;tK2
cG
tW3
a=xV-1;goto
lD1;}
tP2
cG(xV);a=0;}
for(;tJ2
t61=info;retry_positionalparams
cM3
TestParam(cU
a),eT3,(iW2)tX2
eM2}
lD1:tV1
info=t61;goto
retry_positionalparams;}
iR3--a;goto
lD1;}
info=lO4
n31
tM3
if(tW2
for(tL2
t63
SaveMatchedParamIndex(a)n31
lX1(true,&*x5)iL3
SelectedParams:case
AnyParams:{xR<t7>x5;cL3
used(eM);std::vector<iG2>lF3(xV);std::vector<iG2>yG2(xV);for(eQ1=0;tJ2
const
cX2
yF2=cU
a);lF3[a]=ParamSpec_GetDepCode(yF2);}
{iG2
b=0;for(tL2
if(lF3[a]!=0)yG2[b++]=a;for(tL2
if(lF3[a]==0)yG2[b++]=a;}
tK2
t7
tW3
if(xV==0){a=0;goto
retry_anyparams_4;}
a=xV-1;goto
e61;}
tP2
t7(xV,eM);a=0;if(xV!=0){lO4=info;lN4
0].used=used;}
}
for(;tJ2
iR3
t61=info;lN4
a].used=used;}
retry_anyparams
cM3
TestParam_AnyWhere
xK(cU
yG2[a]),tree,(iW2,used,tW2
tX2
eM2}
e61:tV1
info=t61;used=lN4
a].used;goto
retry_anyparams;}
e71:iR3--a;goto
e61;}
info=lO4
n31
tM3
retry_anyparams_4:if(nN.n1!=0
eD3!TopLevel||!t63
HasRestHolder(nN.n1)){eH
cD2;cD2.y13
eM);for
nT2
b=0;b<c8++b
eD3
cN3
cP2;cD2.push_back
y91
b));cN3=true
iP
b);}
if(!t63
SaveOrTestRestHolder(nN.n1,cD2)){goto
e71;}
}
else{iI2&cD2=t63
GetRestHolderValues(nN.n1)cV3
0;a<cD2.xM3;++a){bool
found=false;for
nT2
b=0;b<c8++b
eD3
cN3
cP2;if(cD2[a]xF
lE2
b))){cN3=true
iP
b);found=true
tF2}
}
if(!found){goto
e71;}
}
}
}
return
lX1(true,xV?&*x5:0)iL3
GroupFunction:c43
return
tM3}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
#include <algorithm>
#include <assert.h>
using
t5;using
yL1;lP3{xR3
eJ
xZ1
const
cX2&yE2
tB
info,bool
inner=true){t73
t83{const
ParamSpec_NumConstant
xK
yL3
cL1
ParamSpec_NumConstant
xK*)yF2.second
n31
CodeTreeImmed(l84
constvalue)iL3
e93:{cO
yL3(cO*)yF2.second
n31
t63
GetParamHolderValue(l84
index)iL3
SubFunction:{cP
lM4
eJ
tree;eB3
l84
l81);for(eQ1=0;a<l74
yD2;++a){eJ
nparam=xZ1
e81
xK
cU3
param_list,a),info,true);lP1
nparam);}
if
cU3
n1!=0){eH
trees(t63
GetRestHolderValues
cU3
n1));tree.AddParamsMove(trees
e53
eM==1){assert(tree.GetOpcode()==cAdd iU3()==cMul iU3()==cMin iU3()==cMax iU3()==cAnd iU3()==cOr iU3()==cAbsAnd iU3()==cAbsOr);tree.xW2
0));}
lI2
eM==0){switch(tZ1
iR2
cAdd:case
cOr:tree=nF1
0));lC
cMul:case
cAnd:tree=nF1
1));c13
c43}
}
if(inner)tree.Rehash()n31
tree;}
}
return
eJ();}
}
yL1{tY1
SynthesizeRule
cL1
eP2
eJ&tree
t53){switch(rule.ruletype
iR2
ProduceNewTree:{tree.Become(xZ1
e81
lE1
0),info,false)tA2
case
ReplaceParams:c13{std::vector<iG2>list=t63
GetMatchedParamIndexes();std::sort(list.iN2
list.end())cV3
list.xM3;a-->0;)tree
tT2
list[a]);for(eQ1=0;a<rule.repl_param_count;++a){eJ
nparam=xZ1
e81
lE1
a),info,true);lP1
nparam);}
c43}
}
}
#endif
#ifdef DEBUG_SUBSTITUTIONS
#include <sstream>
#include <cstring>
using
lP3
FUNCTIONPARSERTYPES;using
lP3
l21;using
t5;using
yL1;lP3
l21{tY1
DumpMatch
cL1
eP2
xZ3,const
tB
info,bool
DidMatch,std::ostream&o){DumpMatch(rule
eQ3,DidMatch?lE4"match"
:lE4"mismatch"
,o);}
tY1
DumpMatch
cL1
eP2
xZ3,const
tB
info,xO3
tZ3,std::ostream&o){static
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
;o<<tZ3<<" (rule "
<<(&rule-grammar_rules)<<")"
<<":\n  Pattern    : "
;{cX2
tmp;tmp.first=SubFunction;ParamSpec_SubFunction
tmp2;tmp2.data=rule
cC2;tmp.second=cA2
tmp2;DumpParam
xK(tmp,o);}
o<<"\n  Replacement: "
;DumpParams
lE1
rule.repl_param_count
tN2
o<<"  Tree       : "
e83
tree
tN2
if(!std::strcmp(tZ3,lE4"match"
))DumpHashes(tree,o)cV3
0;a<t63
cE
xM3;++a
eD3!t63
paramholder_matches[a].IsDefined()cP2;o<<"           "
<<ParamHolderNames[a]<<" = "
e83
t63
paramholder_matches[a]tN2}
iA1
t63
lQ.xM3;++b
eD3!tY3
tE3
cP2
cV3
0;a<tY3
tF3.xM3;++a){o<<"         <"
<<b<<"> = "
e83
tY3
tF3[a],o);o<<xV3}
}
o<<std::flush;}
}
#endif
#include <list>
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3{xN1
MarkIncompletes(FPoptimizer_CodeTree::cZ2{if(tree.Is_Incompletely_Hashed(tI1;bool
iR1=false;nB1
iR1|=MarkIncompletes
y91
a)e53
iR1)tree.Mark_Incompletely_Hashed()n31
iR1;}
tY1
FixIncompletes(FPoptimizer_CodeTree::cZ2{if(tree.Is_Incompletely_Hashed()){nB1
FixIncompletes
y91
a));tree
eT1}
}
}
t5{lB
Sort()t13
Sort();}
lB
Rehash(bool
constantfolding
eD3
constantfolding)ConstantFolding(*this);else
Sort();data
x7
xR3
cU2
cA{cO3
n82
cP3
yD1=0;
#if 0
long
double
value=Value;e5=crc32::calc(cL1
iG2
char*)&value,cK3
value));key^=(key<<24);
#elif 0
union{cU2{iG2
char
filler1[16];n82
v;iG2
char
filler2[16];}
buf2;cU2{iG2
char
filler3[cK3
n82)+16-c
K3
x91)];e5;}
buf1;}
data;memset(&data,0,cK3
data));data.buf2.v=Value;e5=data.buf1.key;
#else
int
exponent;n82
nL2=std::frexp(Value,&xE2;e5=nT2
tB2+0x8000)&0xFFFF
e53
nL2<0){nL2=-nL2;key=key^0xFFFF;}
else
key+=0x10000;nL2-=n82(0.5);key<<=39;key|=lY1(nL2+nL2)*n82(1u<<31))<<8;
#endif
lP
#ifdef FP_SUPPORT_COMPLEX_NUMBERS
nR1
T
cE2
std::complex<T> >{cO3
std::complex<T>cP3
cA<T>::nF3
cF2,Value.real());nB
fphash_t
temp;cA<T>::nF3
temp,Value.imag());yD1^=temp.hash2;cF2.hash2^=temp.hash1;}
}
;
#endif
#ifdef FP_SUPPORT_LONG_INT_TYPE
i21
cE2
long>{yH
long
Value){e5=Value;lP
#endif
#ifdef FP_SUPPORT_GMP_INT_TYPE
i21
cE2
GmpInt>{cO3
GmpInt
cP3
e5=Value.toInt();lP
#endif
tY1
xF2
xK::Recalculate_Hash_NoRecursion(){fphash_t
cF2(lY1
Opcode)<<56,Opcode*iY3(0x1131462E270012B));Depth=1;switch(Opcode
iR2
cImmed:{cA
xK::nF3
cF2,Value
tA2
case
l53:{yD1|=lY1
cQ1<<48
lF1((lY1
cQ1)*11)^iY3(0x3A83A83A83A83A0)tF2}
case
cFCall:case
cPCall:{yD1|=lY1
cQ1<<48
lF1((~lY1
cQ1)*7)^3456789;}
c13{size_t
eR1=0
cV3
0;a<l33
xM3;++a
eD3
l43
y02>eR1)eR1=l43
y02;yD1+=((l43
i02
hash1*(a+1))>>12)lF1
l43
i02
hash1
lF1(3)*iY3(0x9ABCD801357);cF2.hash2*=iY3(0xECADB912345)lF1(~l43
i02
hash2)^4567890;}
Depth+=eR1;}
}
if(Hash!=cF2){Hash=cF2;l32=0;}
}
lB
iN3{MarkIncompletes(*this);FixIncompletes(*this);}
}
#endif
#include <cmath>
#include <list>
#include <cassert>
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3{using
t5
nJ1
bool
x01
xZ3,long
count,const
xH1::SequenceOpCode
xK&eU,xH1::i33&synth,size_t
max_bytecode_grow_length);static
const
cU2
SinCosTanDataType{OPCODE
whichopcode;OPCODE
inverse_opcode;enum{nominator,nM2,inverse_nominator,lG1}
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
,{cG2{cSinh,cCosh,cH2,{cSinh,cNop,{cG2
cNop,cCosh}
}
,{cCosh,cNop,{cSinh,cG2
cNop}
}
,{cNop,cTanh,{cCosh,cSinh,cH2,{cNop,cSinh,{cNop,cTanh,cCosh,cNop}
}
,{cNop,cCosh,{cTanh,cSinh,cH2}
;}
t5{lB
SynthesizeByteCode(std::vector<iG2>&ByteCode,std::vector
xK&Immed,size_t&stacktop_max){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Making bytecode for:\n"
;iR
#endif
while(RecreateInversionsAndNegations()){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"One change issued, produced:\n"
;iR
#endif
iN3;using
yL1;using
lP3
l21;const
void*g=cA2
grammar_optimize_recreate;while(ApplyGrammar(*cL1
Grammar*)g,*this)){iN3;}
}
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Actually synthesizing, after recreating inv/neg:\n"
;iR
#endif
xH1::i33
synth;SynthesizeByteCode(synth,false);synth.Pull(ByteCode,Immed,stacktop_max);}
lB
SynthesizeByteCode(xH1::i33&synth,bool
MustPopTemps
lB4{y01*this))yR;}
for
iB1
a=0;a<12;++a){const
SinCosTanDataType&data=SinCosTanData[a];if(data.whichopcode!=cNop)iD1!=data.whichopcode
cP2;CodeTree
nG3;nG3.lN1
nG3
cQ3
inverse_opcode);nG3.n42
y01
nG3)){synth.l11
else
iD1!=cInv
cP2;if(GetParam(0)nC!=data.inverse_opcode
cP2;y01
GetParam(0))){synth.l11
size_t
found[4];iA1
4;++b){CodeTree
tree;if(data.i03]==cNop){eB3
cInv);CodeTree
nH3;nH3.lN1
nH3
cQ3
i03^2]);nH3.n42
lP1
nH3);}
else{tree.lN1
tree
cQ3
i03]);}
tree.n42
found[b]=synth.y33
tree);}
if(found[data.yH2!=tL
nM2]y5
yH2);n81
nM2
i5
cDiv
nK1
yH2!=tL
lG1]y5
yH2);n81
lG1
i5
cMul
nK1
lR1!=tL
lG1]y5
lR1);n81
lG1
i5
cRDiv
nK1
lR1!=tL
nM2]y5
lR1);n81
nM2
i5
cMul,2,1);synth.l11
size_t
n_subexpressions_synthesized=SynthCommonSubExpressions(synth);switch(iS1{case
l53:synth.PushVar(GetVar());lC
cImmed:lG2
GetImmed());lC
cAdd:case
cMul:case
cMin:case
cMax:case
cAnd:case
cOr:case
cAbsAnd:case
cAbsOr:iD1==cMul){bool
cR3=false;yI
lS1
i71&&isLongInteger(lS1
nC1)){c52=makeLongInteger(lS1
nC1);CodeTree
tmp(*this,xG3
CodeTree::CloneTag());tmp
t31
tH3
if(x01
tmp,value,xH1::tN1
xK::AddSequence,synth,MAX_MULI_BYTECODE_LENGTH)){cR3=true
tF2}
}
}
if(cR3)c43
int
yE1=0;cL3
done(GetParamCount(),false);CodeTree
iF;iF
xD
iS1;for(;;){bool
found=false;yI
done[a]cP2;if(synth.IsStackTop(lS1)){found=true;done[a]=true;lS1.n7
iF
c9
lS1
e53++yE1>1){synth
yJ
2);iF.Rehash(false)cM1
iF);yE1=yE1-2+1;}
}
}
if(!found)c43
yI
done[a]cP2;lS1.n7
iF
c9
lS1
e53++yE1>1){synth
yJ
2);iF.Rehash(false)cM1
iF);yE1=yE1-2+1;}
}
if(yE1==0){switch(iS1{case
cAdd:case
cOr:case
cAbsOr:lG2
0);lC
cMul:case
cAnd:case
cAbsAnd:lG2
1);lC
cMin:case
cMax:lG2
0)tF2
c13
c43++yE1;}
assert(n_stacked==1)tF2}
case
cPow:{lQ3
p0=GetParam(0);lQ3
p1=GetParam(1
e53!p1
i71||!isLongInteger(p1
nC1)||!x01
p0,makeLongInteger(p1
nC1),xH1::tN1
xK::MulSequence,synth,MAX_POWI_BYTECODE_LENGTH)){p0.n7
p1.n7
synth
yJ
2);i81
cIf:case
cAbsIf:{xG3
xH1::i33::IfData
ifdata;GetParam(0).n7
synth.SynthIfStep1(ifdata,iS1;GetParam(1).n7
synth.SynthIfStep2(ifdata);GetParam(2).n7
synth.SynthIfStep3(ifdata
tA2
case
cFCall:case
cPCall:{for
iB1
a=0;a<xF1++a)lS1.n7
synth
yJ
nT2)GetParamCount());lT3
nJ2|GetFuncNo(),0,0
tA2
c13{for
iB1
a=0;a<xF1++a)lS1.n7
synth
yJ
nT2)GetParamCount()tA2}
synth.StackTopIs(*this
e53
MustPopTemps&&n_subexpressions_synthesized>0){size_t
top=synth.GetStackTop();synth.DoPopNMov(top-1-n_subexpressions_synthesized,top-1);}
}
}
lP3{xN1
x01
xZ3,long
count,const
xH1::SequenceOpCode
xK&eU,xH1::i33&synth,size_t
max_bytecode_grow_length
eD3
count!=0){xH1::i33
backup=synth;tree.n7
size_t
bytecodesize_backup=synth.GetByteCodeSize();xH1::x01
count
tG2
size_t
bytecode_grow_amount=synth.GetByteCodeSize()-bytecodesize_backup;if(bytecode_grow_amount>max_bytecode_grow_length){synth=backup
n31
tM3
return
true;}
else{xH1::x01
count,eU,synth)xH2}
}
#endif
#include <cmath>
#include <cassert>
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3{using
t5;
#define FactorStack std::vector
const
cU2
PowiMuliType{iG2
opcode_square;iG2
opcode_cumulate;iG2
opcode_invert;iG2
opcode_half;iG2
opcode_invhalf;}
iseq_powi={cSqr,cMul,cInv,cSqrt,cRSqrt}
,iseq_muli={iP1
xE
cNeg,iP1,iP1}
nJ1
n82
eS1
cL1
PowiMuliType&cS3,const
std
yJ3,n62&stack){n82
xA3(1);while(IP<limit
eD3
iT1==cS3.opcode_square
eD3!isInteger
xO1
2;yW
opcode_invert){xA3=-xA3;yW
opcode_half
eD3
xA3>y21&&isEvenInteger
xO1
n82(0.5);yW
opcode_invhalf
eD3
xA3>y21&&isEvenInteger
xO1
n82(-0.5);++IP;eM2
size_t
x12=IP;n82
lhs(1
e53
iT1==cFetch){nA1=nA2;if(index<y2||size_t(index-y2)>=iV1){IP=x12
tF2}
lhs=stack[index-y2];goto
yI2;}
if(iT1==cDup){lhs=xA3;goto
yI2;yI2:eX1
xA3);++IP;n82
subexponent=eS1(cS3
tS
if(IP>=limit||iT1!=cS3.opcode_cumulate){IP=x12
tF2}
++IP;stack.pop_back();xA3+=lhs*subexponent;eM2
c43
return
xA3;}
xR3
n82
ParsePowiSequence
cL1
std
yJ3){n62
stack;eX1
n82(1))n31
eS1(iseq_powi
tS}
xR3
n82
ParseMuliSequence
cL1
std
yJ3){n62
stack;eX1
n82(1))n31
eS1(iseq_muli
tS}
xR3
class
CodeTreeParserData{t23
lE3
CodeTreeParserData(bool
k_powi):stack(),clones(),keep_powi(k_powi){}
void
Eat
iB1
tV3,OPCODE
opcode){eJ
xI;xI
xD
opcode);eH
c02=Pop(tV3)tQ1
c02
e53!keep_powi)switch(opcode
iR2
cTanh:{eJ
sinh,cosh;sinh
xD
cSinh);sinh
c9
xI
lQ2
sinh
eT1
cosh
xD
cCosh);cosh
yP1
xI
lQ2
cosh
eT1
eJ
pow;pow
xD
cPow);pow
yP1
cosh);pow.yC
n82(-1)));pow
eT1
xI
eA3
xI.nG1
0,sinh);xI
yP1
pow
tA2
case
cTan:{eJ
sin,cos;sin
xD
cSin);sin
c9
xI
lQ2
sin
eT1
cos
xD
cCos);cos
yP1
xI
lQ2
cos
eT1
eJ
pow;pow
xD
cPow);pow
yP1
cos);pow.yC
n82(-1)));pow
eT1
xI
eA3
xI.nG1
0,sin);xI
yP1
pow
tA2
case
cPow:{lC2
p0=xI
l8
0);lC2
p1=xI
l8
1
e53
p1
nC==cAdd){eH
xF3(p1.GetParamCount())cV3
0;a<p1.xF1++a){eJ
pow;pow
xD
cPow);pow
c9
p0);pow
c9
p1
nK3
pow
eT1
xF3[a
i13
pow);}
xI
xD
cMul)tQ1
nR3;}
c43
c13
c43
xI.Rehash(!keep_powi);iU1,false);
#ifdef DEBUG_SUBSTITUTIONS
eZ1<<tV3<<", "
<<eE3
opcode)<<"->"
<<eE3
xI
nC)<<": "
l44
xI
tT
xI);
#endif
eX1
c53
EatFunc
iB1
tV3,OPCODE
t43
iG2
funcno
tD3
CodeTreeFuncOp
xK(t43
funcno);eH
c02=Pop(tV3)tQ1
c02);xI.n42
#ifdef DEBUG_SUBSTITUTIONS
eZ1<<tV3<<", "
l44
xI
tT
xI);
#endif
iU1);eX1
c53
AddConst(yJ1
tD3
CodeTreeImmed(value);iU1);Push(c53
AddVar
nT2
varno
tD3
CodeTreeVar
xK(varno);iU1);Push(c53
SwapLastTwoInStack(){eY1
1
i13
eY1
2])c63
Dup(){Fetch(iV1-1)c63
Fetch
iB1
which){Push(stack[which]);}
nR1
T>void
Push(T
tree){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<l44
tree
tT
tree);
#endif
eX1
tree)c63
PopNMov
iB1
target,size_t
source){stack[target]=stack[source];stack.t33
target+1);}
eJ
yJ2{clones.clear();eJ
xA3(stack.back());stack.t33
iV1-1)n31
xA3;}
eH
Pop
iB1
n_pop){eH
xA3(n_pop);for
nT2
n=0;n<n_pop;++n
nP3[n
i13
eY1
n_pop+n]);
#ifdef DEBUG_SUBSTITUTIONS
for
iB1
n=n_pop;n-->0;){eZ1
e83
xA3[n]tT
xA3[n]);}
#endif
stack.t33
iV1-n_pop)n31
xA3;}
size_t
GetStackTop(yZ1
iV1;}
private:void
FindClone(eJ&,bool=true)yR;}
private:eH
stack;std::multimap<fphash_t,eJ>clones;bool
keep_powi;private:CodeTreeParserData
cL1
CodeTreeParserData&);CodeTreeParserData&eA1=cL1
CodeTreeParserData&);}
nJ1
cU2
IfInfo{eJ
eD2;eJ
thenbranch
iM3
endif_location;IfInfo():eD2(),thenbranch(),endif_location(){}
}
;}
t5{lB
GenerateFrom
cL1
xG3
FunctionParserBase
xK::Data&xC3,bool
keep_powi){eH
xA2;xA2.y13
xC3.mVariablesAmount);for
nT2
n=0;n<xC3.mVariablesAmount;++n){xA2.push_back(CodeTreeVar
xK(n+l53));}
GenerateFrom(xC3,xA2,keep_powi);}
lB
GenerateFrom
cL1
xG3
FunctionParserBase
xK::Data&xC3,const
x1&xA2,bool
keep_powi){const
std::vector<iG2>&ByteCode=xC3.mByteCode;const
std::vector
xK&Immed=xC3.mImmed;
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"ENTERS GenerateFrom()\n"
;
#endif
CodeTreeParserData
xK
sim(keep_powi);std::vector<IfInfo
xK>eG;for
iB1
IP=0,DP=0;;++IP){i12:while(!eG
cT3&&(eG.e6==IP||(IP<i8
xM3&&iT1==cJump&&eG.tD1.IsDefined()))){CodeTree
elsebranch=sim.yJ2
nC2
eG.back().eD2)nC2
eG.tD1)nC2
elsebranch);iX2
3,cIf);eG.pop_back();}
if(IP>=i8
xM3)break;iG2
opcode=iT1;if((opcode==cSqr||opcode==cDup||(opcode==cInv&&!IsIntType
xK::xA3)||opcode==cNeg||opcode==cSqrt||opcode==cRSqrt||opcode==cFetch)){size_t
was_ip=IP;n82
e32
ParsePowiSequence
xK(ByteCode,IP,eG
cT3?i8
xM3:eG.e6,sim.xM
1);if
tB2!=n82(1.0)){xL
exponent
xB2;goto
i12;}
if(opcode==cDup||opcode==cFetch||opcode==cNeg){n82
xQ2=ParseMuliSequence
xK(ByteCode,IP,eG
cT3?i8
xM3:eG.e6,sim.xM
1
e53
xQ2!=n82(1.0)){xL
xQ2)y4
cMul);goto
i12;}
}
IP=was_ip;}
if(n72>=l53){nA1=opcode-l53
nC2
xA2[index]);}
else{switch(n72
iR2
cIf:case
cAbsIf:{eG.t33
eG.xM3+1);CodeTree
res(sim.yJ2);eG.back().eD2.swap(res);eG.e6=i8
xM3;IP+=2;eM2
case
cJump:{CodeTree
res(sim.yJ2);eG.tD1.swap(res);eG.e6=lO3
IP+1]+1;IP+=2;eM2
case
cImmed:xL
Immed[DP++]);lC
cDup:sim.Dup();lC
cNop:lC
cFCall:{iG2
funcno=nA2;assert(funcno<fpdata.mFuncPtrs.size());iG2
c02=xC3.mFuncPtrs[funcno].mParams;sim.EatFunc(c02,n72,funcno
tA2
case
cPCall:{iG2
funcno=nA2;assert(funcno<fpdata.l24.size());const
FunctionParserBase
xK&p=*xC3.l24[funcno].mParserPtr;iG2
c02=xC3.l24[funcno].mParams;x1
paramlist=sim.Pop(c02);CodeTree
i22;i22.GenerateFrom(*p.mData,paramlist)nC2
i22
tA2
case
cInv:xL
1
cB2
cDiv);lC
cNeg
nN2
cNeg)tF2
xL
0
cB2
cSub);lC
cSqr:xL
2
xB2;lC
cSqrt:xL
n82(t01
cRSqrt:xL
n82(-t01
cCbrt:xL
n82(1)/n82(3)xB2;lC
cDeg:xL
fp_const_rad_to_deg
eU1
cRad:xL
fp_const_deg_to_rad
eU1
cExp:cR1)goto
iY2;xL
fp_const_e
xK()cB2
cPow);lC
cExp2:cR1)goto
iY2;xL
2.0
cB2
cPow);lC
cCot
nN2
cTan
nW
cCsc
nN2
cSin
nW
cSec
nN2
cCos
nW
cInt:
#ifndef __x86_64
cR1){sim
n92
cInt
tA2
#endif
xL
n82(0.5))cX3;sim
n92
cFloor);lC
cLog10
nN2
cY3
fp_const_log10inv
eU1
cLog2
nN2
cY3
fp_const_log2inv
eU1
eL3:lA4
cY3
fp_const_log2inv
xK());iX2
3,cMul);lC
cHypot:xL
2
xB2;i53
xL
2
xB2
cX3;xL
n82(t01
cSinCos:sim.Dup();sim
n92
cSin);lA4
cCos);lC
cSinhCosh:sim.Dup();sim
n92
cSinh);lA4
cCosh);lC
cRSub:i53
case
cSub:cR1){iX2
2,cSub
tA2
xL-1)y4
cMul)cX3;lC
cRDiv:i53
case
cDiv:cR1||IsIntType
xK::xA3){iX2
2,cDiv
tA2
xL-1
xB2
y4
cMul);lC
cAdd:case
cMul:case
cMod:case
cPow:case
cEqual:case
cLess:case
cGreater:case
cNEqual:case
cLessOrEq:case
cGreaterOrEq:case
cAnd:case
cOr:case
cAbsAnd:case
cAbsOr:iX2
2,t11
lC
cNot:case
cNotNot:case
e03:case
cAbsNotNot
nN2
t11
lC
cFetch:sim.Fetch(nA2);lC
cPopNMov:{iG2
stackOffs_target=nA2;iG2
stackOffs_source=nA2;sim.PopNMov(stackOffs_target,stackOffs_source
tA2
c13
iY2:;iG2
funcno=opcode-cAbs;assert(funcno<FUNC_AMOUNT);const
FuncDefinition&func=Functions[funcno];iX2
func.c02,t11
c43}
}
Become(sim.yJ2);
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Produced tree:\n"
;iR
#endif
}
}
#endif
#include <algorithm>
#ifdef FP_SUPPORT_OPTIMIZER
#include <assert.h>
#define FP_MUL_COMBINE_EXPONENTS
lP3{using
lP3
FUNCTIONPARSERTYPES;using
t5
nJ1
static
void
AdoptChildrenWithSameOpcode(cZ2{
#ifdef DEBUG_SUBSTITUTIONS
bool
nO2=false;
#endif
for
iY
if
y91
a)nC==tZ1){
#ifdef DEBUG_SUBSTITUTIONS
if(!nO2)yA2"Before assimilation: "
yA1
nO2=true;}
#endif
tree.AddParamsMove
y91
a).GetUniqueRef().l02),a);}
#ifdef DEBUG_SUBSTITUTIONS
if(nO2)yA2"After assimilation:   "
yA1}
#endif
}
}
t5{tY1
ConstantFolding(cZ2{tree.Sort();
#ifdef DEBUG_SUBSTITUTIONS
void*cZ3=0
iS2"["
<<(&cZ3)<<"]Runs ConstantFolding for: "
yA1
DumpHashes(tree
n52
std::flush;
#endif
if(false){redo:;tree.Sort();
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"["
<<(&cZ3)<<"]Re-runs ConstantFolding: "
yA1
DumpHashes(tree);
#endif
}
if(tZ1!=cImmed){yV3
p=iN
tree
e53
p
n51&&p
nP2
tQ2==e23){xO
p
tQ2);nD}
if(false){ReplaceTreeWithOne:xO
n82(1));goto
do_return;ReplaceTreeWithZero:xO
xD1;goto
do_return;ReplaceTreeWithParam0:
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Before replace: "
iS2
std::hex<<'['
lD2
hash1<<','
lD2
hash2<<']'<<std::dec
yA1
#endif
tree.xW2
0));
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"After replace: "
iS2
std::hex<<'['
lD2
hash1<<','
lD2
hash2<<']'<<std::dec
yA1
#endif
cZ
lG3
tZ1
iR2
cImmed:lC
l53:lC
cAnd:case
cAbsAnd:eQ
bool
c2=false;for
iY{if(!n13
a)))c2=true;yY3
a),tZ1==cAbsAnd)iR2
IsNever
eL
IsAlways:tree
t31);lC
lT1
lG3
eM
iR2
0:i6
1:eB3
tZ1==cAnd?cNotNot
lI3
cZ
c13
lH3
cAnd||!c2)if(ConstantFolding_AndLogic(eE2
i81
cOr:case
cAbsOr:eQ
bool
c2=false;for
iY{if(!n13
a)))c2=true;yY3
a),tZ1==cAbsOr))l92
i6
lL3
tree
t31);lC
lT1
lG3
eM
iR2
0
eL
1:eB3
tZ1==cOr?cNotNot
lI3
cZ
c13
lH3
cOr||!c2)if(ConstantFolding_OrLogic(eE2
i81
cNot:case
e03:{iG2
lZ1
0;switch
y91
0)nC
iR2
cEqual:lZ1
cNEqual;lC
cNEqual:lZ1
cEqual;lC
cLess:lZ1
cGreaterOrEq;lC
cGreater:lZ1
cLessOrEq;lC
cLessOrEq:lZ1
cGreater;lC
cGreaterOrEq:lZ1
cLess;lC
cNotNot:lZ1
cNot;lC
cNot:lZ1
cNotNot;lC
e03:lZ1
cAbsNotNot;lC
cAbsNotNot:lZ1
e03
tF2
c13
c43
if(opposite){eB3
OPCODE(opposite));tree.SetParamsMove
y91
0).GetUniqueRef().l02));cZ
lG3
tM
0),tree
cS1
iR2
IsAlways
eL
lL3
i6
lT1
lH3
cNot&&GetPositivityInfo
y91
0))==IsAlways)eB3
e03);lY3
nC==cIf||lE2
0)nC==yO3{eJ
lJ3=lE2
0);lC2
ifp1=lJ3
l8
1);lC2
ifp2=lJ3
l8
2
e53
ifp1
nC==cNot||ifp1
cS1{tree
xJ
ifp1
nC==cNot?cNotNot
lI3
p1
e13
lQ2
p1
i63
eJ
p2;p2
iQ2
p2
tI3
tN
if(ifp2
nC==cNot||ifp2
cS1{tree
xJ
tZ1);p1
e13);p1
i63
eJ
p2;p2
xD
ifp2
nC==cNot?cNotNot
lI3
p2
tI3
l8
0)tN
i81
cNotNot:case
cAbsNotNot:{if(n13
0)))nI3
yY3
0),tZ1==cAbsNotNot)iR2
IsNever
eL
IsAlways:i6
lT1
lH3
cNotNot&&GetPositivityInfo
y91
0))==IsAlways)eB3
cAbsNotNot);lY3
nC==cIf||lE2
0)nC==yO3{eJ
lJ3=lE2
0);lC2
ifp1=lJ3
l8
1);lC2
ifp2=lJ3
l8
2
e53
ifp1
nC==cNot||ifp1
cS1{cH1
0,lJ3
lQ2
tree
e13);eJ
p2;p2
iQ2
p2
tI3
tN
if(ifp2
nC==cNot||ifp2
cS1{tree
xJ
tZ1);p1
e13);p1
i63
tree
tI3);eB3
lJ3
nC);cZ}
i81
cIf:case
cAbsIf:{if(ConstantFolding_IfOperations(eE2
c43
case
cMul:{NowWeAreMulGroup:;AdoptChildrenWithSameOpcode(tree);n82
nL1=n82(1)iM3
iW1=0;bool
nM1=false;nB1{if(!lE2
t21
continue;n82
immed=eU3;if(immed==xD1
goto
ReplaceTreeWithZero;nL1*=immed;++iW1;}
if(iW1>1||(iW1==1&&yM3
nL1
tG1
1))))nM1=true;if(nM1){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"cMul: Will add new "
l34
nL1<<"\n"
;
#endif
for
iY
if
y91
t21{
#ifdef DEBUG_SUBSTITUTIONS
xT3<<" - For that, deleting "
l34
eU3
iS2"\n"
;
#endif
i73!yM3
nL1
tG1
1
i83
c9
e21
nL1));lG3
eM
iR2
0:i6
1:nI3
c13
if(ConstantFolding_MulGrouping(eE2
if(ConstantFolding_MulLogicItems(eE2
i81
cAdd:eQ
n82
nD2=0.0
iM3
iW1=0;bool
nM1=false;nB1{if(!lE2
t21
continue;n82
immed=eU3;nD2+=immed;++iW1;}
if(iW1>1||(iW1==1&&nD2==xD1)nM1=true;if(nM1){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"cAdd: Will add new "
l34
nD2<<"\n"
iS2"In: "
yA1
#endif
for
iY
if
y91
t21{
#ifdef DEBUG_SUBSTITUTIONS
xT3<<" - For that, deleting "
l34
eU3
iS2"\n"
;
#endif
i73!(nD2==n82(0.0
i83
c9
e21
nD2));lG3
eM
iR2
0
eL
1:nI3
c13
if(ConstantFolding_AddGrouping(eE2
if(ConstantFolding_AddLogicItems(eE2
i81
cMin:eQ
size_t
yK2=0;yV3
e0;nB1{while(a+1<eM&&eT3
xF
lE2
a+1
i83
t31+1);yW3
max
lK3(!e0
yN
known||(e23)<e0
yN
val)){e0
yN
val=e23;e0
yN
known=true;yK2=a;}
}
if(e0
xD2
for
iY{yW3
min
lK3
a!=yK2&&p
tQ2>=e0
yN
val)i73
eM==1){nI3
i81
cMax:eQ
size_t
yK2=0;yV3
eZ;nB1{while(a+1<eM&&eT3
xF
lE2
a+1
i83
t31+1);yW3
min
lK3(!eZ
n51||p
tQ2>eZ
tQ2)){eZ
tQ2=p
tQ2;eZ
n51=true;yK2=a;}
}
if(eZ
n51){for
iY{yW3
max
lK3
a!=yK2&&(e23)<eZ
tQ2){eF2}
}
if(eM==1){nI3
i81
cEqual:case
cNEqual:case
cLess:case
cGreater:case
cLessOrEq:case
cGreaterOrEq:if(ConstantFolding_Comparison(eE2
lC
cAbs:{yV3
p0
tI
0)e53
p0
n51
n03
nI3
if(p0
eN{tree
eA3
tree.yC
n82(1)));goto
NowWeAreMulGroup;}
lY3
nC==cMul){lC2
p=lE2
0);eH
nJ3;eH
cI2
cV3
0;a<p.xF1++a){p0=iN
p
nK3
if(p0
n51
n03{nJ3.push_back(p
nK3}
if(p0
eN{cI2.push_back(p
nK3}
}
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"Abs: mul group has "
<<nJ3.xM3<<" pos, "
<<cI2.xM3<<"neg\n"
;
#endif
if(!nJ3
cT3||!cI2
cT3){
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"AbsReplace-Before: "
e83
tree
n52"\n"
<<std::flush;DumpHashes(tree
i32);
#endif
eJ
eZ3;eZ3
xD
cMul)cV3
0;a<p.xF1++a){p0=iN
p
nK3
if((p0
n51
n03||(p0
eN){}
else
eZ3
c9
p
nK3}
eZ3
eT1
eJ
nL3;nL3
xD
cAbs);nL3
yP1
eZ3);nL3
eT1
eJ
y41
cMul);xF3
yP1
nL3);yQ1
AddParamsMove(nJ3
e53!cI2
cT3
eD3
cI2.xM3%2)yQ1
yC
n82(-1)));yQ1
AddParamsMove(cI2);}
tree.Become
tD2);
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"AbsReplace-After: "
e83
tree
i32
n52"\n"
<<std::flush;DumpHashes(tree
i32);
#endif
goto
NowWeAreMulGroup;}
}
c43
#define HANDLE_UNARY_CONST_FUNC(funcname) nR){xO funcname(lR));nD
case
cLog:iT3(fp_log);lY3
nC==cPow){eJ
pow=lE2
0
e53
GetPositivityInfo(pow
l8
0))==IsAlways){pow.CopyOnWrite()i7
lH4
if(GetEvennessInfo(pow
yQ3==IsAlways){pow.l61
eJ
abs;abs
xD
cAbs);abs
yP1
pow
lQ2
abs.Rehash()i7
pow.nG1
0,abs);lH4}
else
lY3
nC==cAbs){eJ
pow=lE2
0)l8
0
e53
pow
nC==cPow){pow.l61
eJ
abs;abs
xD
cAbs);abs
yP1
pow
lQ2
abs.Rehash()i7
pow.nG1
0,abs);lH4}
lC
cAcosh:iT3(fp_acosh);lC
cAsinh:iT3(fp_asinh);lC
cAtanh:iT3(fp_atanh);lC
cAcos:iT3(fp_acos);lC
cAsin:iT3(fp_asin);lC
cAtan:iT3(fp_atan);lC
cCosh:iT3(fp_cosh);lC
cSinh:iT3(fp_sinh);lC
cTanh:iT3(fp_tanh);lC
cSin:iT3(fp_sin);lC
cCos:iT3(fp_cos);lC
cTan:iT3(fp_tan);lC
cCeil:if(n5
iT3(fp_ceil);lC
cTrunc:if(n5
iT3(fp_trunc);lC
cFloor:if(n5
iT3(fp_floor);lC
cInt:if(n5
iT3(fp_int);lC
cCbrt:iT3(fp_cbrt);lC
cSqrt:iT3(fp_sqrt);lC
cExp:iT3(fp_exp);lC
cLog2:iT3(fp_log2);lC
cLog10:iT3(fp_log10);lC
eL3
lF4
fp_log2(lR)*tH
i82
cArg:iT3(fp_arg);lC
cConj:iT3(fp_conj);lC
cImag:iT3(fp_imag);lC
cReal:iT3(fp_real);lC
cPolar
lF4
fp_polar(x22
cMod
lF4
fp_mod(x22
cAtan2:{yV3
p0
tI
yP2
p1
tI
1));nR&&yM3
lR,xD1
eD3
p1
cJ2
p1
yN
val)<xD1{xO
fp_const_pi
e33
if(p1
n51&&p1
tQ2>=t51
xD1;nD}
tR2
yM3
tH,xD1
eD3
p0
cJ2
p0
yN
val)<xD1{xO-fp_const_pihalf
e33
if(p0
n51&&p0
tQ2>xD1{xO
fp_const_pihalf
e33}
if
lI
fp_atan2(lR,tH));nD
if((p1
n51&&p1
tQ2>xD1||(p1
cJ2
p1
yN
val)<fp_const_negativezero
xK())){eJ
yL2;yL2
xD
cPow);yL2.AddParamMove
y91
1));yL2.yC
n82(-1)));yL2
eT1
eJ
yM2;yM2
eA3
yM2.AddParamMove
y91
0));yM2
yP1
yL2);yM2
eT1
eB3
cAtan);tree.nG1
0,yM2
iV2
1);i81
cPow:{if(ConstantFolding_PowOperations(eE2
c43
case
cDiv:nR&&eW3
i71&&tH!=t51
lR/tH
i82
cInv:nR&&lR!=t51
n82(1)/lR
i82
cSub
lF4
lR-tH
i82
cNeg:nR){xO-lR
i82
cRad:nR){xO
RadiansToDegrees
lI4
cDeg:nR){xO
DegreesToRadians
lI4
cSqr:nR){xO
lR*lR
i82
cExp2:iT3(fp_exp2);lC
cRSqrt:nR){xO
n82(1)/fp_sqrt
lI4
cCot:i92
fp_tan(lZ
cSec:i92
fp_cos(lZ
cCsc:i92
fp_sin(lZ
cHypot
lF4
fp_hypot(x22
cRDiv:case
cRSub:case
cDup:case
cFetch:case
cPopNMov:case
cSinCos:case
cSinhCosh:case
cNop:case
cJump:lC
cPCall:case
cFCall:c43
do_return:;
#ifdef DEBUG_SUBSTITUTIONS
xT3<<"["
<<(&cZ3)<<"]Done ConstantFolding, result: "
yA1
DumpHashes(tree);
#endif
}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
t5{tY1
yV3::set_abs(nL
bool
has_negative=!min.known||min.val<n82();bool
has_positive=!eC3||i93>n82();bool
crosses_axis=has_negative&&has_positive;cE1
xK
newmax;if(min
lK3
eC3)newmax.set(fp_max(i31,i41
e53
crosses_axis)min.set(n82());e43
min
lK3
eC3)min.set(fp_min(i31,i41);lI2
min.known)min.set(i31);else
min.set(i41;}
max=newmax;}
tY1
yV3::set_neg(){std::swap(min,max);min.val=-min.val;i93=-i93;}
xN1
IsLogicalTrueValue
cL1
yV3&p
tY2{if(nB
IsIntType
xK::xA3
eD3
p
n51&&p
tQ2>=n82(1
tI1;if(!abs&&p
nP2
yN
val<=n82(-1
tI1;}
e43
p
n51&&p
tQ2>=n82(0.5
tI1;if(!abs&&p
nP2
yN
val<=n82(-0.5
tI1;}
return
tM3
xN1
IsLogicalFalseValue
cL1
yV3&p
tY2{if(nB
IsIntType
xK::xA3
eD3
abs)return
p
yN
known
lH1
1);else
return
p
n51&&p
nP2
tQ2>n82(-1)lH1
1);}
e43
abs)return
p
yN
known
lH1
0.5);else
return
p
n51&&p
nP2
tQ2>n82(-0.5)lH1
0.5);}
}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;using
t5;t5{xR3
yV3
iN
xZ3)
#ifdef DEBUG_SUBSTITUTIONS_extra_verbose
{using
lP3
FUNCTIONPARSERTYPES;yV3
tmp=CalculateResultBoundaries_do(tree
n52"Estimated boundaries: "
;if(tmp
n51)xT3<<tmp
tQ2;else
xT3<<"-inf"
iS2" .. "
;if(tmp
xD2
xT3<<tmp
yN
val;else
xT3<<"+inf"
iS2": "
e83
tree
n52
std::endl
n31
tmp;}
xR3
yV3
eJ::CalculateResultBoundaries_do
cL1
cZ2
#endif
{iG
yF1(-fp_const_pihalf
xK(),fp_const_pihalf
xK());iG
pi_limits(-fp_const_pi
xK(),fp_const_pi
xK());iG
abs_pi_limits(y21,fp_const_pi
xK());iG
plusminus1_limits(n82(-c73
using
lP3
std;switch(tZ1
iR2
cImmed:nM
tree
nC1,tree
nC1);case
cAnd:case
cAbsAnd:case
cOr:case
cAbsOr:case
cNot:case
e03:case
cNotNot:case
cAbsNotNot:case
cEqual:case
cNEqual:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:{nM
y21
tG1
1))iL3
cAbs:lD
m.set_abs();tV
cLog:lD
nM3
fp_log
nN3
fp_log);tV
cLog2:lD
nM3
fp_log2
nN3
fp_log2);tV
cLog10:lD
nM3
fp_log10
nN3
fp_log10);tV
cAcosh:lD
yK
template
set_if<cGreaterOrEq
t71
fp_acosh)xQ1
cGreaterOrEq
t71
fp_acosh);tV
cAsinh:lD
yK
set(fp_asinh);m
yN
set(fp_asinh);tV
cAtanh:lD
yK
n3-1),fp_atanh)xQ1
cLess
t71
fp_atanh);tV
cAcos:lD
nM(c71&&(c81)<n82(1))?fp_acos(c81):y21,(nO3&&tC2)>=n82(-1))?fp_acos
tC2):fp_const_pi
xK())iL3
cAsin:lD
yK
n3-1),fp_asin,yF1
tQ2)xQ1
cLess
t71
fp_asin,yF1
yN
val);tV
cAtan:lD
yK
set(fp_atan,yF1
tQ2);m
yN
set(fp_atan,yF1
yN
val);tV
cAtan2:{nR&&yM3
lR,xD1)yR
abs_pi_limits;}
tR2
yM3
tH,xD1)yR
yF1;}
return
pi_limits
iL3
cSin:lD
bool
x11=!nO3||!c71||(c81-yK
val)>=(yO
x11)eR
n82
min=fp_mod
tC2,yO
min<xD1
min
yP
n82
max=fp_mod(c81,yO
max<xD1
max
yP
if(max<min)max
yP
bool
y11=(min<=fp_const_pihalf
xK()&&max>=fp_const_pihalf
xK());bool
nN1=(min<=cD&&max>=cD
e53
y11&&nN1)eR
if(nN1)nM
n82(-1)lJ2
if(y11)nM
eG2
tG1
1));nM
eG2
lJ2}
case
cCos:lD
if(nO3)yK
val+=fp_const_pihalf
xK(e53
c71)c81+=fp_const_pihalf
xK();bool
x11=!nO3||!c71||(c81-yK
val)>=(yO
x11)eR
n82
min=fp_mod
tC2,yO
min<xD1
min
yP
n82
max=fp_mod(c81,yO
max<xD1
max
yP
if(max<min)max
yP
bool
y11=(min<=fp_const_pihalf
xK()&&max>=fp_const_pihalf
xK());bool
nN1=(min<=cD&&max>=cD
e53
y11&&nN1)eR
if(nN1)nM
n82(-1)lJ2
if(y11)nM
eG2
tG1
1));nM
eG2
lJ2}
case
cTan:{nM)iL3
cCeil:lD
c91
cFloor:lD
tH1
tV
cTrunc:lD
tH1
c91
cInt:lD
tH1
c91
cSinh:lD
yK
set(fp_sinh);m
yN
set(fp_sinh);tV
cTanh:lD
yK
set(fp_tanh,plusminus1_limits.min);m
yN
set(fp_tanh,plusminus1_limits.max);tV
cCosh:lD
if(nO3
eD3
c71){if
tC2>=y21&&c81>=xD1{yK
val
xU}
lI2
tC2)<y21&&c81>=xD1{n82
tmp
xU
if(tmp>c81)c81=tmp;yK
val=n82(1);}
else{yK
val
xU
std::swap
tC2,c81);}
}
else{if
tC2>=xD1{m
t0
yK
val=fp_cosh
tC2);}
else{m
t0
yK
val=n82(1);}
}
}
else{nO3=true;yK
val=n82(1
e53
c71){yK
val=fp_cosh(c81);m
t0}
else
m
t0}
tV
cIf:case
cAbsIf:{yV3
res1
tI
1));yV3
res2
tI
2)e53!res2
n51)res1
n51=false;lI2
res1
n51&&(res2
tQ2)<res1
tQ2)res1
tQ2=res2
tQ2;if(!res2
xD2
res1
t0
lI2
res1
cJ2
res2
yN
val)>res1
yN
val)res1
yN
val=res2
yN
val
n31
res1
iL3
cMin:{bool
iH=false;bool
iI=false;yX3;x2
m
tI
a)e53!nO3)iH
iP2
n51||tC2)<nT3
iA3=yK
val;if(!c71)iI
iP2
yN
known||(c81)<nU3
nP3
yN
val=c81;}
if(iH
nP3
n51=false;if(iI
nP3
t0
return
nV3
cMax:{bool
iH=false;bool
iI=false;yX3;x2
m
tI
a)e53!nO3)iH
iP2
n51||yK
val>nT3
iA3=yK
val;if(!c71)iI
iP2
yN
known||c81>nU3
nP3
yN
val=c81;}
if(iH
nP3
n51=false;if(iI
nP3
t0
return
nV3
cAdd:{yX3(y21,xD1;x2
item
tI
a)e53
item
n51
iA3+=item
tQ2;else
nS3=false;if(item
xD2
nU3+=item
yN
val;else
xA3
t0
if(!nS3&&!xA3
xD2
c43
if(nS3&&x13&&nT3>nU3)std::swap
tO2
val,nU3)n31
nV3
cMul:{cU2
Value{enum
nX3{eH3,iX1,i42}
;nX3
tK;n82
value;Value(nX3
t):tK(t),value(0){}
Value(n82
v):tK(eH3),value(v){}
bool
cK2
yZ1
tK==iX1||cL2
value<xD1
c63
eA1*=cL1
Value&rhs){if
cL2
rhs.tK==eH3)value*=rhs.value;else
tK=(cK2)!=rhs.cK2)?iX1:i42);}
iH2<cL1
Value&rhs
yZ1(tK==iX1&&rhs.tK!=iX1)||cL2(rhs.tK==i42||(rhs.tK==eH3&&value<rhs.value)));}
}
;cU2
yG1{Value
yN2,yO2;yG1():yN2(Value::i42),yO2(Value::iX1){}
void
xC2
Value
eI3,const
Value&value2){eI3*=value2;if(eI3<yN2)yN2=eI3;if(yO2<eI3)yO2=eI3;}
}
;yX3(n82(c73
x2
item
tI
a)e53!item
n51&&!item
xD2
nM);Value
nY3=nS3?Value
tO2
lU1
iX1);Value
nZ3=x13?Value(x03
lU1
i42);Value
x23=item
n51?Value(item.min.lU1
iX1);Value
x33=item
yN
known?Value(item
yN
lU1
i42);yG1
range;range.xC2
nY3,x23);range.xC2
nY3,x33);range.xC2
nZ3,x23);range.xC2
nZ3,x33
e53
range.yN2.tK==Value::eH3
iA3=range.yN2.value;else
nS3=false;if(range.yO2.tK==Value::eH3
nP3
yN
val=range.yO2.value;else
xA3
t0
if(!nS3&&!xA3
xD2
c43
if(nS3&&x13&&nT3>nU3)std::swap
tO2
val,nU3)n31
nV3
cMod:{yV3
x
tI
yP2
y
tI
1)e53
y
xD2{if(y
yN
val>=xD1{if(!x
n51||(x
tQ2)<xD1
nM-y
yN
val,y
yN
val);else
nM
y21,y
yN
val);}
e43!x
yN
known||(x
yN
val)>=xD1
nM
y
yN
val,-y
yN
val);else
nM
y
yN
val,fp_const_negativezero
xK());}
}
else
nM)iL3
cPow:{tR2
tH==xD1{nM
n82(c73}
nR&&lR==xD1{nM
y21,xD1;}
nR&&yM3
lR
x83
nM
n82(c73}
tR2
tH>y21&&GetEvennessInfo
y91
1))==IsAlways){n82
e32
tH;yV3
tmp
tI
yP2
xA3;nS3=true;nT3=0;if(tmp
n51&&tmp
tQ2>=xD1
nT3
eF3
tmp
tQ2,xE2;lI2
tmp
yN
known&&tmp
yN
val<=xD1
nT3
eF3
tmp
yN
val,xE2;xA3
t0
if(tmp
n51&&tmp
xD2{x13=true;nU3=fp_max(fp_abs(tmp
tQ2),fp_abs(tmp
yN
val));nU3
eF3
nU3,xE2;}
return
xA3;}
yV3
p0
tI
yP2
p1
tI
1))nW3
p0_positivity=(p0
n51&&(p0
tQ2)>=xD1?IsAlways:(p0
cJ2
p0
yN
val)<y21?lL3
Unknown)nW3
cM2=GetEvennessInfo
y91
1))nW3
t1=Unknown;switch(p0_positivity)l92
t1=IsAlways;lC
lL3{t1=cM2
tF2}
c13
switch(cM2)l92
t1=IsAlways;lC
lL3
lC
Unknown:{tR2!isInteger
tG&&tH>=xD1{t1=IsAlways;}
c43}
lG3
t1)l92{n82
min=y21;if(p0
n51&&p1
n51){min
eF3
p0
tQ2,p1
tQ2
e53
p0
tQ2<y21&&(!p1
yN
known||p1
yN
val>=xD1&&min>=xD1
min=y21;}
if(p0
n51&&p0
tQ2>=y21&&p0
yN
known&&p1
xD2{n82
max
eF3
p0
yN
val,p1
yN
val
e53
min>max)std::swap(min,max);nM
min,max);}
nM
min,false)iL3
lL3{nM
false,fp_const_negativezero
xK());}
c13{c43
i81
cNeg:lD
m.set_neg();tV
cSub:{yM
cNeg);iB3
1));tmp
xD
cAdd);tmp.nJ
0));tmp
yP1
tmp2)t03
cInv:{cN2-1
yQ2
cDiv:{yM
cInv);iB3
1));tmp
xD
x71
AddParamMove(tmp2)t03
cRad:{t81
x71
yC
fp_const_rad_to_deg
xK(yQ2
cDeg:{t81
x71
yC
fp_const_deg_to_rad
xK(yQ2
cSqr:{cN2
2
yQ2
cExp:{t81
cPow);tmp.yC
fp_const_e
xK()));tmp.nJ
0))t03
cExp2:{t81
cPow);tmp.yC
x43
tmp.nJ
0))t03
cCbrt:lD
yK
set(fp_cbrt);m
yN
set(fp_cbrt);tV
cSqrt:lD
if(nO3)yK
val=tC2)<y21?0:fp_sqrt
tC2
e53
c71)c81=(c81)<y21?0:fp_sqrt(c81);tV
cRSqrt:{cN2-0.5
yQ2
cHypot:{eJ
xsqr,ysqr,add,sqrt;xsqr.nJ
0));xsqr.yC
x43
ysqr.nJ
1));ysqr.yC
x43
xsqr
xD
cPow);ysqr
xD
cPow);add
yP1
xsqr);add
yP1
ysqr);add
xD
cAdd);sqrt
yP1
add);sqrt
xD
cSqrt)n31
iN
sqrt)iL3
eL3:{yM
cLog2);iB3
0));tmp
eA3
tmp
yP1
tmp2);tmp.nJ
1))t03
cCot:{yM
cTan)x3
lH
cSec:{yM
cCos)x3
lH
cCsc:{yM
cSin)x3
iN
tmp);}
lC
cRDiv:case
cRSub:case
cDup:case
cFetch:case
cPopNMov:case
cSinCos:case
cSinhCosh:case
cNop:case
cJump:case
l53:lC
cArg:case
cConj:case
cImag:case
cReal:case
cPolar:lC
cPCall:lC
cFCall:c43
nM);}
xR3
TriTruthValue
GetIntegerInfo
cL1
cZ2{switch(tZ1
iR2
cImmed:return
eV2
tree
nC1)?IsAlways:IsNever;case
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
cNEqual:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:return
IsAlways;case
cIf:{TriTruthValue
a=GetIntegerInfo
y91
1))nW3
b=GetIntegerInfo
y91
2)e53
a==b)return
a
nV2
case
cAdd:case
cMul:{for
iY
if(GetIntegerInfo
y91
a))!=IsAlways)return
Unknown
n31
IsAlways;}
c13
c43
return
Unknown;}
xN1
IsLogicalValue
cL1
cZ2{switch(tZ1
iR2
cImmed:return
yM3
tree
nC1,xD1||yM3
tree
nC1
tG1
1));case
cAnd:case
cOr:case
cNot:case
cNotNot:case
cAbsAnd:case
cAbsOr:case
e03:case
cAbsNotNot:case
cEqual:case
cNEqual:case
cLess:case
cLessOrEq:case
cGreater:case
cGreaterOrEq:nV
cMul:{for
iY
if(!n13
a))cL
return
true
iL3
cIf:case
cAbsIf:yR
n13
1))nT1
lE2
2));}
c13
c43
return
tM3}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;
#if defined(__x86_64) || __cplusplus < 201100
# define CBRT_IS_SLOW
#endif
#if defined(DEBUG_POWI) || defined(DEBUG_SUBSTITUTIONS)
#include <cstdio>
#endif
lP3
xH1{extern
const
iG2
char
powi_table[256];}
lP3{using
t5
nJ1
bool
IsOptimizableUsingPowi(long
immed,long
penalty=0){xH1::i33
synth;synth.PushVar(l53)iM3
bytecodesize_backup=synth.GetByteCodeSize();xH1::x01
immed,xH1::tN1
xK::MulSequence,synth)iM3
bytecode_grow_amount=synth.GetByteCodeSize()-bytecodesize_backup
n31
bytecode_grow_amount<size_t(MAX_POWI_BYTECODE_LENGTH-penalty);}
tY1
ChangeIntoRootChain(eJ&tree,bool
lM3,long
i52,long
i62){while(i62>0){t81
cCbrt);iC3
tmp
eT1
tree.i72--i62;}
while(i52>0){t81
cSqrt
e53
lM3){tmp
xD
cRSqrt);lM3=tM3
iC3
tmp
eT1
tree.i72--i52;}
if(lM3){t81
cInv);iC3
tree.i72}
}
xR3
cU2
RootPowerTable{static
const
n82
RootPowers[(1+4)*(1+3)];}
nJ1
const
n82
t8(1+4)*(1+3)]={n82(1)lS
lJ4
lJ4
2*lJ4
i01)lS
3*2)lS
3*2*2)lS
3*i01*2*i01*3
eO2
2
eO2
2*2
eO2
i01*3*2*i01*3*3
eO2
3*2
eO2
3*2*2
eO2
3*i01*3*3*2*2*2*2)}
;cU2
PowiResolver{static
const
iG2
MaxSep=4;static
x63
iD3=5;typedef
int
eJ3;typedef
long
xD3;typedef
long
tW;cU2
yR2{yR2():n_int_sqrt(0),n_int_cbrt(0),sep_list(),lV1(0){}
int
n_int_sqrt;int
n_int_cbrt;int
tL1
MaxSep];tW
lV1;}
nJ1
static
yR2
CreatePowiResult(n82
xE2{yR2
xA3;eJ3
t9=FindIntegerFactor(xE2;if(t9==0){
#ifdef DEBUG_POWI
iA2"no factor found for %Lg\n"
,yK1
xE2;
#endif
return
xA3;}
eK3=y31
exponent,t9);xD3
eH2=EvaluateFactorCost(t9,0,0,0)+cB
eK3);int
iE3=0;int
iF3=0;int
mul_count=0;
#ifdef DEBUG_POWI
iA2"orig = %Lg\n"
,yK1
xE2;iA2"plain factor = "
l54"%ld\n"
,(int)t9,(long)eH2);
#endif
for
nT2
n_s=0;n_s<MaxSep;++n_s){int
x9=0;xD3
yI1=eH2;eJ3
yW1=t9;for(int
s=1;s<iD3*4;++s){
#ifdef CBRT_IS_SLOW
if(s>=iD3)break;
#endif
int
n_sqrt=s%iD3;int
n_cbrt=s/iD3;if(n_sqrt+n_cbrt>4
cP2;n82
lI1=exponent;lI1-=t8
s];i51=FindIntegerFactor(lI1
e53
xQ2!=0){tW
xP=y31
lI1,xQ2);xD3
cost=EvaluateFactorCost(xQ2,iE3+n_sqrt,iF3+n_cbrt,mul_count+1)+cB
xP);
#ifdef DEBUG_POWI
iA2"Candidate sep %u (%d*sqrt %d*cbrt)factor = "
l54"%ld (for %Lg to %ld)\n"
,s,n_sqrt,n_cbrt,xQ2,(long)cost,yK1
lI1,(long)xP);
#endif
if(cost<yI1){x9=s;yW1=xQ2;yI1=cost;}
}
}
if(!x9)break;
#ifdef DEBUG_POWI
iA2"CHOSEN sep %u (%d*sqrt %d*cbrt)factor = "
l54"%ld, exponent %Lg->%Lg\n"
,x9,x9%iD3,x9/iD3,yW1,yI1,yK1(xE2,yK1
tB2-t8
x9]));
#endif
x53
tL1
n_s]=x9;exponent-=t8
x9];iE3+=x9%iD3;iF3+=x9/iD3;eH2=yI1;t9=yW1;mul_count+=1;}
eK3=y31
exponent,t9);
#ifdef DEBUG_POWI
iA2"resulting exponent is %ld (from exponent=%Lg, best_factor=%Lg)\n"
,eK3,yK1
exponent,yK1
t9);
#endif
while(t9%2==0){++xA3
tI2;t9/=2;}
while(t9%3==0){++x53
n_int_cbrt;t9/=3;}
return
xA3;}
private:static
xD3
cB
tW
xP){static
std::map
cO2
iB;if(xP<0){xD3
cost=22
n31
cost+cB-xP);}
std::map
cO2::yN3
i=iB.xR2
xP
e53
i!=iB.cY1
xP)return
i
e22;std::pair
cO2
xA3(xP,0.0);xD3&cost=x53
iI3
while(xP>1){int
xQ2=0;if(xP<256){xQ2=xH1::powi_table[xP];if(xQ2&128)xQ2&=127;else
xQ2=0;if(xQ2&64)xQ2=-(xQ2&63)-1;}
if(xQ2){cost+=cB
xQ2);xP/=xQ2;eM2
if(!(xP&1)){xP/=2;cost+=6;}
else{cost+=7;xP-=1;}
}
iB.yD3
i,xA3)n31
cost;}
cA1
tW
y31
yJ1,i51)yR
makeLongInteger(value*n82(xQ2));}
cA1
bool
yR1
yJ1,i51){n82
v=value*n82(xQ2)n31
isLongInteger(v);}
cA1
eJ3
FindIntegerFactor(yJ1){i51=(2*2*2*2);
#ifdef CBRT_IS_SLOW
#else
xQ2*=(3*3*3);
#endif
eJ3
xA3=0;if(yR1
value,xQ2)){xA3=xQ2;while((xQ2%2)==0&&yR1
value,xQ2/2)nP3=xQ2/=2;while((xQ2%3)==0&&yR1
value,xQ2/3)nP3=xQ2/=3;}
#ifdef CBRT_IS_SLOW
if(xA3==0
eD3
yR1
value,3
lB3
3;}
#endif
return
xA3;}
static
int
EvaluateFactorCost(int
xQ2,int
s,int
c,int
nmuls){x63
x73=6;
#ifdef CBRT_IS_SLOW
x63
eI2=25;
#else
x63
eI2=8;
#endif
int
xA3=s*x73+c*eI2;while(xQ2%2==0){xQ2/=2;xA3+=x73;}
while(xQ2%3==0){xQ2/=3;xA3+=eI2;}
xA3+=nmuls
n31
xA3;}
}
;}
t5{xN1
eJ::RecreateInversionsAndNegations(bool
prefer_base2){bool
changed=false
cV3
0;a<xF1++a)if(lS1.RecreateInversionsAndNegations(prefer_base2))x42
if(changed){exit_changed:Mark_Incompletely_Hashed()xH2
switch(iS1{case
cMul:{eH
nE2;eJ
nF2,cT1;if(true){bool
nO1=false;n82
x32=0;cW3
yS2
0)cQ2
tO
1)i71){nO1=true;x32=tO
1)nC1
tF2}
}
if(nO1){n82
immeds=1.0;cW3
i71){immeds*=powgroup
nC1;t41}
for
iJ-->0;){eJ&powgroup=lS1;if(powgroup
yS2
0)cQ2
tO
1)i71){eJ&log2=tO
0);log2.l61
log2
xD
eL3);log2.yC
fp_pow(immeds
tG1
1)/x32)));log2
eT1
c43}
}
}
cW3
yS2
1)i71){lC2
exp_param=tO
1);n82
e32
exp_param
nC1;if(e01
tG1-1))){l61
nE2.push_back(lS1
lQ2
t41
lI2
exponent<y21&&eV2
xE2){eJ
iK;iK
xD
cPow);iK
c9
tO
0));iK.yC-xE2);iK
eT1
nE2.push_back(iK);l61
t41}
lI2
powgroup
cQ2!nF2.t91
nF2=tO
0);l61
t41
lI2
powgroup
nC==eL3&&!cT1.t91
cT1=powgroup;l61
t41}
if(!nE2
cT3){x42
eJ
i11;i11
eA3
i11.SetParamsMove(nE2);i11
eT1
eJ
y41
cMul);yQ1
SetParamsMove
eF
if(yQ1
IsImmed()&&fp_equal
tD2
nC1
x83
nG2
cInv)iG3;}
e43
yQ1
y02>=i11.y02){nG2
cDiv)eO
nR3
iG3;}
else{nG2
cRDiv)iG3
eO
nR3;}
}
}
if(nF2.t91
eJ
y41
iS1;yQ1
SetParamsMove
eF
while(yQ1
RecreateInversionsAndNegations(prefer_base2))yQ1
iN3;nG2
eL3)eO
nF2)eO
nR3;x42}
if(cT1.t91
eJ
y41
cMul);xF3
yP1
cT1
yQ3;yQ1
AddParamsMove
eF
while(yQ1
RecreateInversionsAndNegations(prefer_base2))yQ1
iN3;DelParams();nG2
eL3)eO
cT1
l8
0))eO
nR3;x42
i81
cAdd:{eH
iB2;for
iJ-->0;)if(eM3
cMul){tS2
y51:;eJ&xF3=x62
for
iB1
b=yQ1
xF1
b-->0;){if
tD2
l8
b)l71
xQ2=xF3
l8
b)nC1
yI3
xQ2
iC2
y51;}
yQ1
l61
yQ1
DelParam(b);lN3
lI2
yM3
xQ2
tG1-2)))xC
y51;}
yQ1
l61
yQ1
DelParam(b);yQ1
yC
x43
lN3}
}
if(t2){yQ1
tA
nR3;t41}
lI2
eM3
cDiv&&!IsIntType
xK::xA3){tS2
y61:;eJ&i11=x62
if(i11
l8
0)i61
yM3
i11
l8
0)nC1
iC2
y61;}
i11.l61
i11
tT2
0);i11
xD
cInv);lN3}
if(t2)xC
y61;}
i11.tA
i11);t41}
lI2
eM3
cRDiv&&!IsIntType
xK::xA3){tS2
x81:;eJ&i11=x62
if(i11
l8
1)i61
yM3
i11
l8
1)nC1
iC2
x81;}
i11.l61
i11
tT2
1);i11
xD
cInv);lN3}
if(t2)xC
x81;}
i11.tA
i11);t41}
if(!iB2
cT3){
#ifdef DEBUG_SUBSTITUTIONS
iA2"Will make a Sub conversion in:\n"
);fflush(stdout);iR
#endif
eJ
yT2;yT2
xD
cAdd);yT2.SetParamsMove(iB2);yT2
eT1
eJ
cU1;cU1
xD
cAdd);cU1.SetParamsMove(l02));cU1
eT1
if(cU1
i71&&yM3
cU1
nC1,xD1){nG2
cNeg);e7);}
e43
cU1.y02==1){nG2
cRSub);e7)tJ3}
lI2
yT2
nC==cAdd){nG2
cSub)tJ3
e7
lQ2
for
iB1
a=1;a<yT2.xF1++a){eJ
eJ2;eJ2
xD
cSub);eJ2.SetParamsMove(l02));eJ2.Rehash(false)eO
eJ2);e7
nK3}
}
else{nG2
cSub)tJ3
e7);}
}
#ifdef DEBUG_SUBSTITUTIONS
iA2"After Sub conversion:\n"
);fflush(stdout);iR
#endif
i81
cPow:{lC2
p0=GetParam(0);lC2
p1=GetParam(1
e53
p1
i61
p1
nC1!=y21&&!eV2
p1
nC1)){eC
yR2
r=eC
CreatePowiResult(fp_abs(p1
nC1)e53
r.lV1!=0){bool
iY1=false;if(p1
nC1<y21&&r.tL1
0]==0&&r
tI2>0){iY1=true;}
#ifdef DEBUG_POWI
iA2"Will resolve powi %Lg as powi(chain(%d,%d),%ld)"
,yK1
fp_abs(p1
nC1),r
tI2,r.n_int_cbrt,r.lV1);for
nT2
n=0;n<eC
MaxSep;++n
eD3
r
lA3==0)break;int
n_sqrt=r
lA3%eC
iD3;int
n_cbrt=r
lA3/eC
iD3;iA2"*chain(%d,%d)"
,n_sqrt,n_cbrt);}
iA2"\n"
);
#endif
eJ
cR2=GetParam(0);eJ
yU2=cR2;yU2.l61
ChangeIntoRootChain(yU2,iY1,r
tI2,r.n_int_cbrt);yU2
eT1
eJ
pow;if(r.lV1!=1){pow
xD
cPow);pow
yP1
yU2);pow.yC
n82(r.lV1)));}
else
pow.swap(yU2);eJ
mul;mul
eA3
mul
yP1
pow);for
nT2
n=0;n<eC
MaxSep;++n
eD3
r
lA3==0)break;int
n_sqrt=r
lA3%eC
iD3;int
n_cbrt=r
lA3/eC
iD3;eJ
eK2=cR2;eK2.l61
ChangeIntoRootChain(eK2,false,n_sqrt,n_cbrt);eK2
eT1
mul
yP1
eK2);}
if(p1
nC1<y21&&!iY1){mul
eT1
nG2
cInv);nG1
0,mul);DelParam(1);}
else{nG2
cMul);SetParamsMove(mul.l02));}
#ifdef DEBUG_POWI
iR
#endif
x42
c43}
}
if(GetOpcode()==cPow&&(!p1
i71||!isLongInteger(p1
nC1)||!IsOptimizableUsingPowi
xK(makeLongInteger(p1
nC1)))eD3
p0
i71&&p0
nC1>n82(0.0)eD3
prefer_base2){n82
yV2=fp_log2(p0
nC1)yI3
yV2
x83
DelParam(0);}
else{n0
e21
yV2)tE1
c9
p1
tE1
eT1
nG1
nD1}
nG2
cExp2);x42}
else{n82
yV2=fp_log(p0
nC1)yI3
yV2
x83
DelParam(0);}
else{n0
e21
yV2)tE1
c9
p1
tE1
eT1
nG1
nD1}
nG2
cExp);x42}
}
lI2
GetPositivityInfo(p0)==IsAlways
eD3
prefer_base2){eJ
log;log
xD
cLog2);log
c9
p0);log
eT1
n0
p1
tE1
yP1
log
tE1
eT1
nG2
cExp2);nG1
nD1
x42}
else{eJ
log;log
xD
cLog);log
c9
p0);log
eT1
n0
p1
tE1
yP1
log
tE1
eT1
nG2
cExp);nG1
nD1
x42}
}
i81
cDiv:{if(GetParam(0)i71&&yM3
GetParam(0)nC1
x83
nG2
cInv);DelParam(0);}
c43
c13
c43
if(changed)goto
exit_changed
n31
changed;}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
using
lP3
FUNCTIONPARSERTYPES;lP3{using
t5;class
e63{size_t
nP1
iM3
eD
iM3
eE
iM3
lJ1
iM3
t3
iM3
t4
iM3
n01;t23
e63():nP1(0),eD(0),eE(0),lJ1(0),t3(0),t4(0),n01(0){}
void
iK3
OPCODE
op){nP1+=1
iJ3
cCos)++eD
iJ3
cSin)++eE
iJ3
cSec)++eD
iJ3
cCsc)++eE
iJ3
cTan)++lJ1
iJ3
cCot)++lJ1
iJ3
cSinh)++t4
iJ3
cCosh)++t3
iJ3
cTanh)++n01;}
size_t
GetCSEscore
l94
size_t
xA3=nP1
n31
xA3;}
int
NeedsSinCos
l94
bool
y71=(nP1==(eD+eE+lJ1)e53(lJ1&&(eE||eD))||(eE&&eD)eD3
y71)return
1
n31
2;}
return
0;}
int
NeedsSinhCosh
l94
bool
y71=(nP1==(t3+t4+n01)e53(n01&&(t4||t3))||(t4&&t3)eD3
y71)return
1
n31
2;}
return
0;}
size_t
MinimumDepth
l94
size_t
n_sincos=std::min(eD,eE)iM3
n_sinhcosh=std::min(t3,t4
e53
n_sincos==0&&n_sinhcosh==0)return
2
n31
1;}
}
nJ1
class
TreeCountType:public
std::multimap<fphash_t,std::pair<e63,eJ> >{}
xY3
FindTreeCounts(tK1&nH2,xZ3,OPCODE
x72,bool
skip_root=false){cV
i=nH2.xR2
y03
e53!skip_root){bool
found=false;for(;i!=nH2.cY1
y03;++i
eD3
tree
xF
i
e22
iO2){i
e22.first.iK3
x72);found=true
tF2}
}
if(!found){e63
count;count.iK3
x72);nH2.yD3
i,std::make_pair(y03,std::make_pair
eR3,tree)));}
}
nB1
FindTreeCounts(nH2,eT3,tZ1);}
cU2
yU{bool
BalanceGood;bool
FoundChild;}
nJ1
yU
lK1
lC2
root,lC2
child
eD3
root
xF
child)){yU
xA3={true,true}
n31
xA3;}
yU
xA3={true,false}
;if(root
nC==cIf||root
nC==yO3{yU
cond
lL4
0
tZ2
yU
xX
lL4
1
tZ2
yU
y6
lL4
2
tZ2
if
x93||xX
yV||y6
yV){xA3
yV=true;}
xA3
e8=((xX
yV==y6
yV)||x93
cS2&&(cond
e8||(xX
yV&&y6
yV))&&(xX
e8||x93
cS2&&(y6
e8||x93
cS2;}
else{bool
iE1=false;bool
nQ1=false;for
iB1
b=root.GetParamCount(),a=0;a<b;++a){yU
tmp
lL4
a
tZ2
if(tmp
yV
nP3
yV=true;if(tmp
e8
lP4)iE1=true;lI2
tmp
yV)nQ1=true;}
if(iE1&&!nQ1
nP3
e8=tM3
return
xA3;}
xN1
lZ3
lV3
iH3
xZ3,const
xH1::i33&synth,const
tK1&nH2){for
iB1
b=eM,a=0;a<b;++a){lC2
leaf=eT3;cV
synth_it;xV2
tK1::const_iterator
i=nH2.yG3
i!=nH2.end();++i
eD3
i->first!=leaf.GetHash()cP2;const
e63&occ
nQ2
first
iM3
score=occ.GetCSEscore();lC2
candidate
nQ2
iI3
if(tM1
candidate)cP2;if(leaf.y02<occ.MinimumDepth()cP2;if(score<2
cP2;if(lK1
iH3
leaf)e8
lP4
cP2
xH2
if(lZ3(iH3
leaf,synth,nH2
tI1;}
return
tM3
xN1
nI2
lV3
yE3,lC2
expr){i91
yE3
l8
a)xF
expr
tI1;i91
nI2(yE3
l8
a),expr
tI1
n31
tM3
xN1
GoodMomentForCSE
lV3
yE3,lC2
expr
eD3
yE3
nC==cIf)return
true;i91
yE3
l8
a)xF
expr
tI1
iM3
iD2=0;i91
nI2(yE3
l8
a),expr))++iD2
n31
iD2!=1;}
}
t5{xR3
size_t
eJ::SynthCommonSubExpressions(xH1::yM1
const{if(GetParamCount()==0)return
0
iM3
stacktop_before=synth.GetStackTop();tK1
nH2;FindTreeCounts(nH2,*this,GetOpcode(),true);for(;;){size_t
yW2=0;
#ifdef DEBUG_SUBSTITUTIONS_CSE
xT3<<"Finding a CSE candidate, root is:"
<<xV3
DumpHashes(*this);
#endif
cV
cs_it(nH2.end());for(cV
j=nH2.yG3
j!=nH2.end();){cV
i(j++);const
e63&occ
nQ2
first
iM3
score=occ.GetCSEscore();xZ3
nQ2
iI3
#ifdef DEBUG_SUBSTITUTIONS_CSE
xT3<<"Score "
<<score<<":\n"
<<std::flush;DumpTreeWithIndent(tree);
#endif
if(tM1
tree))xT
if(tree.y02<occ.MinimumDepth())xT
if(score<2)xT
if(lK1*this,tree)e8
lP4)xT
if(lZ3(*this,tree,synth,nH2)){eM2
if(!GoodMomentForCSE(*this,tree))xT
score*=tree.y02;if(score>yW2){yW2=score;cs_it=i;}
}
if(yW2<=0){
#ifdef DEBUG_SUBSTITUTIONS_CSE
xT3<<"No more CSE candidates.\n"
<<std::flush;
#endif
c43
xZ3=cs_it
e22.iI3
#ifdef DEBUG_SUBSTITUTIONS_CSE
xT3<<lE4"Common Subexpression:"
;DumpTree
xK(tree
n52
xV3
#endif
#if 0
int
lW1=occ.NeedsSinCos();int
i1=occ.NeedsSinhCosh();eJ
iE2,iF2,yX2,yY2;if(lW1){iE2
eL2
iE2
xD
cSin);iE2
eT1
iF2
eL2
iF2
xD
cCos);iF2
eT1
if(tM1
iE2)||tM1
iF2)eD3
lW1==2){tJ1
eM2
lW1=0;}
}
if(i1){yX2
eL2
yX2
xD
cSinh);yX2
eT1
yY2
eL2
yY2
xD
cCosh);yY2
eT1
if(tM1
yX2)||tM1
yY2)eD3
i1==2){tJ1
eM2
i1=0;}
}
#endif
tree.SynthesizeByteCode(synth,false);tJ1
#ifdef DEBUG_SUBSTITUTIONS_CSE
synth.template
Dump<0>(n52"Done with Common Subexpression:"
;DumpTree
xK(tree
n52
xV3
#endif
#if 0
if(lW1
eD3
lW1==2||i1){synth.e91}
lT3
cSinCos,1,2)cM1
iE2,1)cM1
iF2,0);}
if(i1
eD3
lW1)synth.e91
if(i1==2){synth.e91}
lT3
cSinhCosh,1,2)cM1
yX2,1)cM1
yY2,0);}
#endif
}
return
synth.xM
stacktop_before;}
}
#endif
#ifdef FP_SUPPORT_OPTIMIZER
tY1
FunctionParserBase
xK::Optimize(){using
t5;l61
eJ
tree;tree.GenerateFrom(*mData);FPoptimizer_Optimize::ApplyGrammars(tree);std::vector<iG2>eN3;std::vector
xK
immed
iM3
stacktop_max=0;tree.SynthesizeByteCode(eN3,immed,stacktop_max
e53
mData->mStackSize!=stacktop_max){mData->mStackSize=iG2(stacktop_max);
#if !defined(FP_USE_THREAD_SAFE_EVAL) && \
    !defined(FP_USE_THREAD_SAFE_EVAL_WITH_ALLOCA)
mData->mStack.t33
stacktop_max);
#endif
}
mData->mByteCode.swap(eN3);mData->mImmed.swap(immed);}
#ifdef FP_SUPPORT_MPFR_FLOAT_TYPE
x8
MpfrFloat
n11
#endif
#ifdef FP_SUPPORT_GMP_INT_TYPE
x8
GmpInt
n11
#endif
#ifdef FP_SUPPORT_COMPLEX_DOUBLE_TYPE
x8
std::complex<double>n11
#endif
#ifdef FP_SUPPORT_COMPLEX_FLOAT_TYPE
x8
std::complex<float>n11
#endif
#ifdef FP_SUPPORT_COMPLEX_LONG_DOUBLE_TYPE
x8
std::complex<long
double>n11
#endif
FUNCTIONPARSER_INSTANTIATE_TYPES
#endif

#endif

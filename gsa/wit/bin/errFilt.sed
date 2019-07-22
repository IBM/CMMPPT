/^This is a comment. This is not a shell script; it is used by a script.$/p
/, line .*: warning: cannot inline .* in this expression$/d
/, line .*: warning: out-of-line copy of .* created$/d
/, line .*: warning: .* not inlined, called twice in an expression$/d
/warning:  ::RCS[a-z]* defined but not used/d
/iostream.h.* warning: copy constructors should take a const  .*\& not a  .*\&/d
/iostream.h.* warning: assignment operators should take a const  .*\& not a  .*\&/d
/rw\/locale.h.* warning: assignment operators should take a const  RWLocaleSnapshot\& not a  RWLocaleSnapshot\&/d
/rw\/cstring.h.* warning: const cast away: const class RWCStringRef\*const -> char \*/d
/rw\/collstr.h.* warning: second or virtual base class RWCString without virtual destructor/d
/rw\/tislist.h.* warning: const cast away: const RWIsvSlink\*-> class  RWTValAssocLink <.*, .*>\*/d
/rw\/slist.h.* warning: const cast away: const class RWSlist\*const -> RWSlist\*/d
/rw\/ivec.h.* warning: const cast away: const int \*-> int \*/d
/rw\/tooldefs.h.* warning: const cast away: const void \*-> unsigned int /d
/RWClient\.h.* warning: const cast away: const void \*->/d
/\.h.* warning:  RWSlist::\~RWSlist() too complex for inlining/d
/RWClient\.h.* warning: copy constructors should take a const  .*\& not a  .*\&/d
/RWClient\.C.* warning: no macro replacement within a string literal/d
/rw\/[a-z]*.h.* warning:  .*() too large for inlining/d
/rw\/[a-z]*.h.* warning:   .*() too complex for inlining/d
/rw\/[a-z]*.h.* warning: cannot inline .*::.*() in this expression/d
/rw\/[a-z]*.h.* warning: cannot inline operator .*() in this expression/d
/rw\/[a-z]*.h.* warning: copy constructors should take a const  .*\& not a  .*\&/d
/ warning:  RWModel::\~RWModel() too complex for inlining$/d
/ warning: cannot inline RWCString::RWCString() in this expression$/d
/ warning: cannot inline .*GSlist.*::.*GSlist.* in this expression$/d
/ warning:  .*GSlist.*::.*GSlist.* too large for inlining$/d
/ warning:  .*GSlist.*::.*GSlist.* too complex for inlining$/d
/ warning: virtual function  .*::.*() cannot be inlined$/d
/ warning:  .*::.* not inlined, called twice in an expression/d
/ warning: can.t take address of inline function .*(), .*() not inlined$/d
/scenario\/src\/[a-zA-z]*.h.* warning: copy constructors should take a const  LgFrScenario\& not a  LgFrScenario\&$/d
/scenario\/src\/explode\.h.* warning: copy constructors should take a const  LgFrLastProblemSolved\& not a  LgFrLastProblemSolved\&$/d
/scenario\/src\/wit\.h.* warning: copy constructors should take a const  LgFrLastProblemSolved\& not a  LgFrLastProblemSolved\&$/d
/scenario\/src\/usrInMod\.h.* warning: copy constructors should take a const  LgFrWorkingImplosionProblem\& not a  LgFrWorkingImplosionProblem\&$/d
/scenario\/src\/msAttrIt\.h.* warning: copy constructors should take a const  LgFrMultiScheduleItemIterator\& not a  LgFrMultiScheduleItemIterator\&$/d
/scenario\/src\/multiSch\.h", line 5.: warning: copy constructors should take a const  LgFrDescriptorSchedulePairFloatRWGSlist& not a  LgFrDescriptorSchedulePairFloatRWGSlist&$/d
/interact\/src\/.*\.h.* warning: copy constructors should take a const  LgFr.*RWGSlist\& not a  LgFr.*RWGSlist\&$/d
/src\/prodSS\.C.*: warning:  lps2 not used$/d
/src\/cumSS\.C", line 37.: warning: result of name expression not used$/d
/src\/timeVecs\.C\", line ..: warning: const cast away: const class LgFrTimeVecFloat\*const -> LgFrTimeVecFloat\*const $/d
/src\/sched\.C\", line 7.: warning: const cast away: const size_t-> size_t\&$/d
/src\/sched\.C\", line [12][178].: warning: const cast away: const class LgFrScheduleFloat\*const -> LgFrScheduleFloat\*$/d
/src\/sched\.C", line [12]..: warning: const cast away: const LgFrItem\*-> RWCollectable\*$/d
/src\/multiSch\.h", line 5.: warning: const cast away: const void \*-> LgFrDescriptorSchedulePairFloat\*$/d
/src\/multiSch\.C", line [5678][0-9].: warning: const cast away: const LgFrDescriptorSchedulePairFloatGSList\*-> LgFrDescriptorSchedulePairFloatGSList\*$/d
/src\/scenATP\.C", line 1[67].: warning: const cast away: const LgFrSetOfParts-> LgFrSetOfParts\&$/d
/src\/scenATP\.C", line 20.: warning: const cast away: const LgFrSetOfDemands-> LgFrSetOfDemands\&$/d
/src\/i[PD].*Set\.h", line 2.: warning: const cast away: const void \*-> LgFrSorting[PD].*ScheduleFloat\*$/d
/src\/i[PD].*Set\.h", line 2.: warning: const cast away: const void \*-> LgFr[PD].*ScheduleStrategy\*$/d
/src\/comMgGui\.h", line 1.: warning: const cast away: const void \*-> LgFrInteractiveScenario\*$/d
/src\/msItemIt\.C", line 2.: warning: const cast away: const LgFrSortingScheduleFloat-> LgFrSortingScheduleFloat\&$/d
/src\/mult.Sch\.C", line .*: warning:  RWBooleanoperator ==(const RWCString\&, const RWCString\&) not inlined, called twice in an expression$/d
/src\/refCount\.h", line .*: warning: assignment operators should take a const  LgFrReferenceCount\& not a  LgFrReferenceCount\&$/d
/src\/attTable\.h", line ..: warning: const cast away: const void \*-> LgFrAttributeInformation\*$/d
/src\/attTable\.h", line ..: warning: copy constructors should take a const  LgFrAttributeInformationRWGSlist\& not a  LgFrAttributeInformationRWGSlist\&$/d
/src\/attTable\.h", line ..: warning: second or virtual base class LgFrAttributeInformationRWGSlist without virtual destructor$/d
/src\/attTable\.C", line [12]..: warning: const cast away: const RWHashDictionary-> RWHashDictionary\&/d
/src\/demdSchP\.C", line ..: warning: const cast away: const LgFrSortingDemandScheduleFloatSmartPointerToConst-> LgFrSortingDemandScheduleFloatSmartPointerToConst\&/d
/src\/demdSchP.C", line ...: warning: const cast away: const LgFrSortingDemandScheduleFloatSmartPointer-> LgFrSortingDemandScheduleFloatSmartPointer\&/d
/src\/demdSchP\.C", line [13]..: warning: const cast away: const LgFrReferenceCount-> LgFrReferenceCount\&/d
/src\/atpTest\.C", line 8..: warning: const cast away: const LgFrScheduleFactory\*-> LgFrScheduleFactory\*$/d
/src\/unitTest\.C", line .*: warning: const cast away: const LgFrScheduleFactory\*-> LgFrScheduleFactory\*$/d
/src\/scenCMRP\.C", line 1..: warning: const cast away: const LgFrSetOfParts-> LgFrSetOfParts\&$/d
/src\/scenCMRP\.C", line [78]..: warning: const cast away: const LgFrScheduleFactory\*-> LgFrScheduleFactory\*$/d
/src\/schdSnFS\.C", line ..: warning: const cast away: const LgFrSortingScheduleFloatSmartPointer-> LgFrSortingScheduleFloatSmartPointer\&$/d
/src\/schdSnSS\.C", line [13]..: warning: result of name expression not used$/d

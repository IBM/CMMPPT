#------------------------------------------------------------------------------
# Make file for building WIT-J's Native Library on Windows SDK.
#------------------------------------------------------------------------------

platform   = winsdk

sep        = /

config_dir = ../../../config/

include ../../../config/p_winsdk.mk

include ../src/appl.mk

#------------------------------------------------------------------------------
# Rule for building the WIT-J C++ native library:
#
# The secondary output files witj.lib and witj.exp are not needed and so they
# are removed.
#------------------------------------------------------------------------------

wit_lib    = ../../../wit/winsdk/wit.lib

witj.dll:	$(lib_objects) $(wit_lib)
		link -dll $(ds_link_flags) $^ -out:witj.dll
		rm witj.lib
		rm witj.exp

#------------------------------------------------------------------------------
# Rule for "make all".
# Note the required tab character on the second line of this rule.
#------------------------------------------------------------------------------

all:		witj.dll
		
#------------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

ArrayJOR.obj: ../src/ArrayJOR.h ../src/JniClient.h ../src/Includes.h
ArrayJOR.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ArrayJOR.obj: ../../../config/dummy_headers/stdlib.h
ArrayJOR.obj: ../../../config/dummy_headers/jni.h ../src/CTVecSupply.h
ArrayJOR.obj: ../../../config/dummy_headers/list ../src/Component.h
ArrayJOR.obj: ../src/JavaAccObj.h ../src/Coordinator.h
ArrayJOR.obj: ../../../config/dummy_headers/string
ArrayJOR.obj: ../../../config/dummy_headers/map
ArrayListJOR.obj: ../src/ArrayListJOR.h ../src/JniClient.h ../src/Includes.h
ArrayListJOR.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ArrayListJOR.obj: ../../../config/dummy_headers/stdlib.h
ArrayListJOR.obj: ../../../config/dummy_headers/jni.h
ArrayListJOR.obj: ../../../config/dummy_headers/vector ../src/ComponentJOR.h
ArrayListJOR.obj: ../src/Coordinator.h ../src/JavaAccObj.h
ArrayListJOR.obj: ../../../config/dummy_headers/string
ArrayListJOR.obj: ../../../config/dummy_headers/map ../src/Part.h
ArrayListJOR.obj: ../src/Component.h ../../../wit/src/wit.h
ArrayListJOR.obj: ../../../config/dummy_headers/stdio.h
ArrayListJOR.obj: ../../../config/dummy_headers/limits.h ../src/Demand.h
ArrayListJOR.obj: ../src/Operation.h ../src/StringVec.h
Att.obj: ../src/Att.h ../src/JavaAccObj.h ../src/Includes.h ../src/Macros.h
Att.obj: ../src/Forwards.h ../src/Utils.h
Att.obj: ../../../config/dummy_headers/stdlib.h
Att.obj: ../../../config/dummy_headers/string ../src/AttBldr.h
Att.obj: ../src/VectorAtts.h ../../../wit/src/wit.h
Att.obj: ../../../config/dummy_headers/stdio.h
Att.obj: ../../../config/dummy_headers/limits.h
Att.obj: ../../../config/dummy_headers/map ../src/Coordinator.h
Att.obj: ../src/Problem.h ../src/Component.h ../src/CTVecSupply.h
Att.obj: ../../../config/dummy_headers/list
AttBldr.obj: ../src/AttBldr.h ../src/VectorAtts.h ../src/Att.h
AttBldr.obj: ../src/JavaAccObj.h ../src/Includes.h ../src/Macros.h
AttBldr.obj: ../src/Forwards.h ../src/Utils.h
AttBldr.obj: ../../../config/dummy_headers/stdlib.h
AttBldr.obj: ../../../config/dummy_headers/string ../../../wit/src/wit.h
AttBldr.obj: ../../../config/dummy_headers/stdio.h
AttBldr.obj: ../../../config/dummy_headers/limits.h
AttBldr.obj: ../../../config/dummy_headers/map ../src/ScalarAtts.h
AttBldr.obj: ../src/StringJOR.h ../src/JniClient.h
AttBldr.obj: ../../../config/dummy_headers/jni.h
AttBldr.obj: ../../../config/dummy_headers/iostream
AttBldrCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
AttBldrCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
AttBldrCOR.obj: ../src/Forwards.h ../src/Utils.h
AttBldrCOR.obj: ../../../config/dummy_headers/stdlib.h
AttBldrCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
AttBldrCOR.obj: ../src/AttBldr.h ../src/VectorAtts.h ../src/Att.h
AttBldrCOR.obj: ../src/JavaAccObj.h ../../../config/dummy_headers/string
AttBldrCOR.obj: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
AttBldrCOR.obj: ../../../config/dummy_headers/limits.h
AttBldrCOR.obj: ../../../config/dummy_headers/map ../src/StringJOR.h
AttCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
AttCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
AttCOR.obj: ../src/Forwards.h ../src/Utils.h
AttCOR.obj: ../../../config/dummy_headers/stdlib.h
AttCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
AttCOR.obj: ../src/Att.h ../src/JavaAccObj.h
AttCOR.obj: ../../../config/dummy_headers/string ../src/AttBldr.h
AttCOR.obj: ../src/VectorAtts.h ../../../wit/src/wit.h
AttCOR.obj: ../../../config/dummy_headers/stdio.h
AttCOR.obj: ../../../config/dummy_headers/limits.h
AttCOR.obj: ../../../config/dummy_headers/map ../src/Component.h
AttCOR.obj: ../src/CTVecSupply.h ../../../config/dummy_headers/list
AttCOR.obj: ../src/ArrayJOR.h ../src/StringJOR.h
BomEntry.obj: ../src/BomEntry.h ../src/Component.h ../src/JavaAccObj.h
BomEntry.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
BomEntry.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
BomEntry.obj: ../src/Part.h ../../../wit/src/wit.h
BomEntry.obj: ../../../config/dummy_headers/stdio.h
BomEntry.obj: ../../../config/dummy_headers/limits.h
BomEntry.obj: ../../../config/dummy_headers/string ../src/Operation.h
BomEntry.obj: ../src/Coordinator.h ../../../config/dummy_headers/map
BomEntryCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
BomEntryCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
BomEntryCOR.obj: ../src/Forwards.h ../src/Utils.h
BomEntryCOR.obj: ../../../config/dummy_headers/stdlib.h
BomEntryCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
BomEntryCOR.obj: ../src/BomEntry.h ../src/Component.h ../src/JavaAccObj.h
BopEntry.obj: ../src/BopEntry.h ../src/Component.h ../src/JavaAccObj.h
BopEntry.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
BopEntry.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
BopEntry.obj: ../src/Part.h ../../../wit/src/wit.h
BopEntry.obj: ../../../config/dummy_headers/stdio.h
BopEntry.obj: ../../../config/dummy_headers/limits.h
BopEntry.obj: ../../../config/dummy_headers/string ../src/Operation.h
BopEntry.obj: ../src/Coordinator.h ../../../config/dummy_headers/map
BopEntryCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
BopEntryCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
BopEntryCOR.obj: ../src/Forwards.h ../src/Utils.h
BopEntryCOR.obj: ../../../config/dummy_headers/stdlib.h
BopEntryCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
BopEntryCOR.obj: ../src/BopEntry.h ../src/Component.h ../src/JavaAccObj.h
CTVecSupply.obj: ../src/CTVecSupply.h ../src/Includes.h ../src/Macros.h
CTVecSupply.obj: ../src/Forwards.h ../src/Utils.h
CTVecSupply.obj: ../../../config/dummy_headers/stdlib.h
CTVecSupply.obj: ../../../config/dummy_headers/list
CaseToken.obj: ../src/CaseToken.h ../src/JavaAccObj.h ../src/Includes.h
CaseToken.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
CaseToken.obj: ../../../config/dummy_headers/stdlib.h ../../../wit/src/wit.h
CaseToken.obj: ../../../config/dummy_headers/stdio.h
CaseToken.obj: ../../../config/dummy_headers/limits.h ../src/Coordinator.h
CaseToken.obj: ../../../config/dummy_headers/string
CaseToken.obj: ../../../config/dummy_headers/map
CaseTokenCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
CaseTokenCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
CaseTokenCOR.obj: ../src/Forwards.h ../src/Utils.h
CaseTokenCOR.obj: ../../../config/dummy_headers/stdlib.h
CaseTokenCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
CaseTokenCOR.obj: ../src/CaseToken.h ../src/JavaAccObj.h
CaseTokenCOR.obj: ../../../wit/src/wit.h
CaseTokenCOR.obj: ../../../config/dummy_headers/stdio.h
CaseTokenCOR.obj: ../../../config/dummy_headers/limits.h
Component.obj: ../src/Component.h ../src/JavaAccObj.h ../src/Includes.h
Component.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Component.obj: ../../../config/dummy_headers/stdlib.h ../src/ComponentJOR.h
Component.obj: ../src/JniClient.h ../../../config/dummy_headers/jni.h
Component.obj: ../src/Coordinator.h ../../../config/dummy_headers/string
Component.obj: ../../../config/dummy_headers/map ../src/Problem.h
Component.obj: ../src/CaseToken.h ../../../wit/src/wit.h
Component.obj: ../../../config/dummy_headers/stdio.h
Component.obj: ../../../config/dummy_headers/limits.h ../src/CTVecSupply.h
Component.obj: ../../../config/dummy_headers/list
ComponentCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
ComponentCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
ComponentCOR.obj: ../src/Forwards.h ../src/Utils.h
ComponentCOR.obj: ../../../config/dummy_headers/stdlib.h
ComponentCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
ComponentCOR.obj: ../src/Component.h ../src/JavaAccObj.h
ComponentCOR.obj: ../src/ComponentJOR.h ../src/CTVecSupply.h
ComponentCOR.obj: ../../../config/dummy_headers/list ../src/ArrayJOR.h
ComponentCOR.obj: ../src/ArrayListJOR.h ../../../config/dummy_headers/vector
ComponentJOR.obj: ../src/ComponentJOR.h ../src/JniClient.h ../src/Includes.h
ComponentJOR.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ComponentJOR.obj: ../../../config/dummy_headers/stdlib.h
ComponentJOR.obj: ../../../config/dummy_headers/jni.h ../src/Problem.h
ComponentJOR.obj: ../src/Component.h ../src/JavaAccObj.h ../src/Part.h
ComponentJOR.obj: ../../../wit/src/wit.h
ComponentJOR.obj: ../../../config/dummy_headers/stdio.h
ComponentJOR.obj: ../../../config/dummy_headers/limits.h
ComponentJOR.obj: ../../../config/dummy_headers/string ../src/Operation.h
ComponentJOR.obj: ../src/BomEntry.h ../../../config/dummy_headers/iostream
Coordinator.obj: ../src/Coordinator.h ../src/JavaAccObj.h ../src/Includes.h
Coordinator.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Coordinator.obj: ../../../config/dummy_headers/stdlib.h
Coordinator.obj: ../../../config/dummy_headers/string
Coordinator.obj: ../../../config/dummy_headers/map ../src/Problem.h
Coordinator.obj: ../src/Component.h ../src/CaseToken.h ../../../wit/src/wit.h
Coordinator.obj: ../../../config/dummy_headers/stdio.h
Coordinator.obj: ../../../config/dummy_headers/limits.h ../src/Part.h
Coordinator.obj: ../src/Demand.h ../src/Operation.h ../src/BomEntry.h
Coordinator.obj: ../src/Substitute.h ../src/BopEntry.h ../src/PggHandler.h
Coordinator.obj: ../src/CTVecSupply.h ../../../config/dummy_headers/list
Coordinator.obj: ../src/ComponentJOR.h ../src/JniClient.h
Coordinator.obj: ../../../config/dummy_headers/jni.h ../src/ArrayListJOR.h
Coordinator.obj: ../../../config/dummy_headers/vector
Coordinator.obj: ../src/PreJavaException.h
CoordinatorCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
CoordinatorCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
CoordinatorCOR.obj: ../src/Forwards.h ../src/Utils.h
CoordinatorCOR.obj: ../../../config/dummy_headers/stdlib.h
CoordinatorCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
CoordinatorCOR.obj: ../src/Coordinator.h ../src/JavaAccObj.h
CoordinatorCOR.obj: ../../../config/dummy_headers/string
CoordinatorCOR.obj: ../../../config/dummy_headers/map
Demand.obj: ../src/Demand.h ../src/Component.h ../src/JavaAccObj.h
Demand.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Demand.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Demand.obj: ../../../config/dummy_headers/string ../src/Part.h
Demand.obj: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
Demand.obj: ../../../config/dummy_headers/limits.h ../src/Operation.h
Demand.obj: ../src/Substitute.h ../src/BopEntry.h ../src/Coordinator.h
Demand.obj: ../../../config/dummy_headers/map ../src/CaseToken.h
Demand.obj: ../src/PggHandler.h ../src/ArrayJOR.h ../src/JniClient.h
Demand.obj: ../../../config/dummy_headers/jni.h ../src/ArrayListJOR.h
Demand.obj: ../../../config/dummy_headers/vector
DemandCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
DemandCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
DemandCOR.obj: ../src/Forwards.h ../src/Utils.h
DemandCOR.obj: ../../../config/dummy_headers/stdlib.h
DemandCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
DemandCOR.obj: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
DemandCOR.obj: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
DemandCOR.obj: ../../../config/dummy_headers/limits.h
DemandCOR.obj: ../../../config/dummy_headers/string ../src/Demand.h
ExceptionHandler.obj: ../src/ExceptionHandler.h ../src/JniClient.h
ExceptionHandler.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
ExceptionHandler.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
ExceptionHandler.obj: ../../../config/dummy_headers/jni.h
ExceptionHandler.obj: ../src/PreJavaException.h
ExceptionHandler.obj: ../../../config/dummy_headers/string
ExceptionHandler.obj: ../../../wit/src/wit.h
ExceptionHandler.obj: ../../../config/dummy_headers/stdio.h
ExceptionHandler.obj: ../../../config/dummy_headers/limits.h
JavaAccObj.obj: ../src/Coordinator.h ../src/JavaAccObj.h ../src/Includes.h
JavaAccObj.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
JavaAccObj.obj: ../../../config/dummy_headers/stdlib.h
JavaAccObj.obj: ../../../config/dummy_headers/string
JavaAccObj.obj: ../../../config/dummy_headers/map
JavaAccObjCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
JavaAccObjCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
JavaAccObjCOR.obj: ../src/Forwards.h ../src/Utils.h
JavaAccObjCOR.obj: ../../../config/dummy_headers/stdlib.h
JavaAccObjCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
JavaAccObjCOR.obj: ../src/ComponentJOR.h ../src/JavaAccObj.h
JavaAccObjCOR.obj: ../src/ArrayListJOR.h ../../../config/dummy_headers/vector
JniClient.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
JniClient.obj: ../src/Forwards.h ../src/Utils.h
JniClient.obj: ../../../config/dummy_headers/stdlib.h
JniClient.obj: ../../../config/dummy_headers/jni.h ../src/JavaAccObj.h
JniHandler.obj: ../src/JniHandler.h ../src/JniClient.h ../src/Includes.h
JniHandler.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
JniHandler.obj: ../../../config/dummy_headers/stdlib.h
JniHandler.obj: ../../../config/dummy_headers/jni.h
MessageMgr.obj: ../src/MessageMgr.h ../src/JavaAccObj.h ../src/Includes.h
MessageMgr.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
MessageMgr.obj: ../../../config/dummy_headers/stdlib.h ../src/StringJOR.h
MessageMgr.obj: ../src/JniClient.h ../../../config/dummy_headers/jni.h
MessageMgr.obj: ../src/CaseToken.h ../../../wit/src/wit.h
MessageMgr.obj: ../../../config/dummy_headers/stdio.h
MessageMgr.obj: ../../../config/dummy_headers/limits.h
MessageMgr.obj: ../../../config/dummy_headers/string
MessageMgrCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
MessageMgrCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
MessageMgrCOR.obj: ../src/Forwards.h ../src/Utils.h
MessageMgrCOR.obj: ../../../config/dummy_headers/stdlib.h
MessageMgrCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
MessageMgrCOR.obj: ../src/MessageMgr.h ../src/JavaAccObj.h ../src/StringJOR.h
Operation.obj: ../src/Operation.h ../src/Component.h ../src/JavaAccObj.h
Operation.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Operation.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Operation.obj: ../../../config/dummy_headers/string ../src/Coordinator.h
Operation.obj: ../../../config/dummy_headers/map ../src/PggHandler.h
Operation.obj: ../src/CaseToken.h ../../../wit/src/wit.h
Operation.obj: ../../../config/dummy_headers/stdio.h
Operation.obj: ../../../config/dummy_headers/limits.h ../src/ArrayJOR.h
Operation.obj: ../src/JniClient.h ../../../config/dummy_headers/jni.h
OperationCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
OperationCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
OperationCOR.obj: ../src/Forwards.h ../src/Utils.h
OperationCOR.obj: ../../../config/dummy_headers/stdlib.h
OperationCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
OperationCOR.obj: ../src/Operation.h ../src/Component.h ../src/JavaAccObj.h
OperationCOR.obj: ../../../config/dummy_headers/string
Part.obj: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
Part.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Part.obj: ../../../config/dummy_headers/stdlib.h ../../../wit/src/wit.h
Part.obj: ../../../config/dummy_headers/stdio.h
Part.obj: ../../../config/dummy_headers/limits.h
Part.obj: ../../../config/dummy_headers/string ../src/Coordinator.h
Part.obj: ../../../config/dummy_headers/map ../src/CaseToken.h
Part.obj: ../src/ArrayJOR.h ../src/JniClient.h
Part.obj: ../../../config/dummy_headers/jni.h
PartCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
PartCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
PartCOR.obj: ../src/Forwards.h ../src/Utils.h
PartCOR.obj: ../../../config/dummy_headers/stdlib.h
PartCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
PartCOR.obj: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
PartCOR.obj: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
PartCOR.obj: ../../../config/dummy_headers/limits.h
PartCOR.obj: ../../../config/dummy_headers/string ../src/Coordinator.h
PartCOR.obj: ../../../config/dummy_headers/map ../src/ArrayListJOR.h
PartCOR.obj: ../../../config/dummy_headers/vector
PggHandler.obj: ../src/PggHandler.h ../src/Includes.h ../src/Macros.h
PggHandler.obj: ../src/Forwards.h ../src/Utils.h
PggHandler.obj: ../../../config/dummy_headers/stdlib.h ../src/Coordinator.h
PggHandler.obj: ../src/JavaAccObj.h ../../../config/dummy_headers/string
PggHandler.obj: ../../../config/dummy_headers/map ../src/Part.h
PggHandler.obj: ../src/Component.h ../../../wit/src/wit.h
PggHandler.obj: ../../../config/dummy_headers/stdio.h
PggHandler.obj: ../../../config/dummy_headers/limits.h ../src/Operation.h
PggHandler.obj: ../src/Substitute.h ../src/BopEntry.h ../src/Demand.h
PggHandler.obj: ../src/ArrayListJOR.h ../src/JniClient.h
PggHandler.obj: ../../../config/dummy_headers/jni.h
PggHandler.obj: ../../../config/dummy_headers/vector
PreJavaException.obj: ../src/PreJavaException.h ../src/Includes.h
PreJavaException.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
PreJavaException.obj: ../../../config/dummy_headers/stdlib.h
PreJavaException.obj: ../../../config/dummy_headers/string
Problem.obj: ../src/Problem.h ../src/Component.h ../src/JavaAccObj.h
Problem.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Problem.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Problem.obj: ../src/Coordinator.h ../../../config/dummy_headers/string
Problem.obj: ../../../config/dummy_headers/map ../src/Demand.h
Problem.obj: ../src/CaseToken.h ../../../wit/src/wit.h
Problem.obj: ../../../config/dummy_headers/stdio.h
Problem.obj: ../../../config/dummy_headers/limits.h ../src/StringVec.h
Problem.obj: ../../../config/dummy_headers/vector ../src/PreJavaException.h
Problem.obj: ../src/ComponentJOR.h ../src/JniClient.h
Problem.obj: ../../../config/dummy_headers/jni.h ../src/StringJOR.h
Problem.obj: ../src/ArrayListJOR.h ../../../config/dummy_headers/iostream
Problem.obj: ../../../config/dummy_headers/string.h
ProblemCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
ProblemCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
ProblemCOR.obj: ../src/Forwards.h ../src/Utils.h
ProblemCOR.obj: ../../../config/dummy_headers/stdlib.h
ProblemCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
ProblemCOR.obj: ../src/Problem.h ../src/Component.h ../src/JavaAccObj.h
ProblemCOR.obj: ../src/Coordinator.h ../../../config/dummy_headers/string
ProblemCOR.obj: ../../../config/dummy_headers/map ../src/ArrayListJOR.h
ProblemCOR.obj: ../../../config/dummy_headers/vector ../src/StringJOR.h
ScalarAtts.obj: ../src/ScalarAtts.h ../src/Att.h ../src/JavaAccObj.h
ScalarAtts.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
ScalarAtts.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
ScalarAtts.obj: ../../../config/dummy_headers/string ../../../wit/src/wit.h
ScalarAtts.obj: ../../../config/dummy_headers/stdio.h
ScalarAtts.obj: ../../../config/dummy_headers/limits.h ../src/Part.h
ScalarAtts.obj: ../src/Component.h ../src/Demand.h ../src/Operation.h
ScalarAtts.obj: ../src/BomEntry.h ../src/Substitute.h ../src/BopEntry.h
ScalarAtts.obj: ../src/StringJOR.h ../src/JniClient.h
ScalarAtts.obj: ../../../config/dummy_headers/jni.h
StringJOR.obj: ../src/StringJOR.h ../src/JniClient.h ../src/Includes.h
StringJOR.obj: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
StringJOR.obj: ../../../config/dummy_headers/stdlib.h
StringJOR.obj: ../../../config/dummy_headers/jni.h
StringVec.obj: ../src/StringVec.h ../src/Includes.h ../src/Macros.h
StringVec.obj: ../src/Forwards.h ../src/Utils.h
StringVec.obj: ../../../config/dummy_headers/stdlib.h
StringVec.obj: ../../../config/dummy_headers/vector
Substitute.obj: ../src/Substitute.h ../src/Component.h ../src/JavaAccObj.h
Substitute.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Substitute.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Substitute.obj: ../src/BomEntry.h ../src/Part.h ../../../wit/src/wit.h
Substitute.obj: ../../../config/dummy_headers/stdio.h
Substitute.obj: ../../../config/dummy_headers/limits.h
Substitute.obj: ../../../config/dummy_headers/string ../src/Coordinator.h
Substitute.obj: ../../../config/dummy_headers/map
SubstituteCOR.obj: ../src/NativeMacros.h ../src/ExceptionHandler.h
SubstituteCOR.obj: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
SubstituteCOR.obj: ../src/Forwards.h ../src/Utils.h
SubstituteCOR.obj: ../../../config/dummy_headers/stdlib.h
SubstituteCOR.obj: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
SubstituteCOR.obj: ../src/Substitute.h ../src/Component.h ../src/JavaAccObj.h
Utils.obj: ../src/PreJavaException.h ../src/Includes.h ../src/Macros.h
Utils.obj: ../src/Forwards.h ../src/Utils.h
Utils.obj: ../../../config/dummy_headers/stdlib.h
Utils.obj: ../../../config/dummy_headers/string ../../../wit/src/wit.h
Utils.obj: ../../../config/dummy_headers/stdio.h
Utils.obj: ../../../config/dummy_headers/limits.h
Utils.obj: ../../../config/dummy_headers/iostream
Utils.obj: ../../../config/dummy_headers/string.h
VectorAtts.obj: ../src/VectorAtts.h ../src/Att.h ../src/JavaAccObj.h
VectorAtts.obj: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
VectorAtts.obj: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
VectorAtts.obj: ../../../config/dummy_headers/string ../../../wit/src/wit.h
VectorAtts.obj: ../../../config/dummy_headers/stdio.h
VectorAtts.obj: ../../../config/dummy_headers/limits.h ../src/Part.h
VectorAtts.obj: ../src/Component.h ../src/Demand.h ../src/Operation.h
VectorAtts.obj: ../src/BomEntry.h ../src/Substitute.h ../src/BopEntry.h
VectorAtts.obj: ../src/ArrayJOR.h ../src/JniClient.h
VectorAtts.obj: ../../../config/dummy_headers/jni.h

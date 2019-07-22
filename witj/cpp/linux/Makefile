#------------------------------------------------------------------------------
# Make file for building the WIT-J Native Library on Linux
#------------------------------------------------------------------------------

platform   = linux

sep        = /

config_dir = ../../../config/

include ../../../config/p_linux.mk


# Compiler flag to specify the output file.
# We just let gcc use its default output file name.
#
OUTPUT_OPTION =


include ../src/appl.mk

include ../src/appl_unix.mk

#------------------------------------------------------------------------------
# Rule for "make all".
# Note the required tab character on the second line of this rule.
#------------------------------------------------------------------------------

all:	libwitj.so
	

#------------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it.

ArrayJOR.o: ../src/ArrayJOR.h ../src/JniClient.h ../src/Includes.h
ArrayJOR.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ArrayJOR.o: ../../../config/dummy_headers/stdlib.h
ArrayJOR.o: ../../../config/dummy_headers/jni.h ../src/CTVecSupply.h
ArrayJOR.o: ../../../config/dummy_headers/list ../src/Component.h
ArrayJOR.o: ../src/JavaAccObj.h ../src/Coordinator.h
ArrayJOR.o: ../../../config/dummy_headers/string
ArrayJOR.o: ../../../config/dummy_headers/map
ArrayListJOR.o: ../src/ArrayListJOR.h ../src/JniClient.h ../src/Includes.h
ArrayListJOR.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ArrayListJOR.o: ../../../config/dummy_headers/stdlib.h
ArrayListJOR.o: ../../../config/dummy_headers/jni.h
ArrayListJOR.o: ../../../config/dummy_headers/vector ../src/ComponentJOR.h
ArrayListJOR.o: ../src/Coordinator.h ../src/JavaAccObj.h
ArrayListJOR.o: ../../../config/dummy_headers/string
ArrayListJOR.o: ../../../config/dummy_headers/map ../src/Part.h
ArrayListJOR.o: ../src/Component.h ../../../wit/src/wit.h
ArrayListJOR.o: ../../../config/dummy_headers/stdio.h
ArrayListJOR.o: ../../../config/dummy_headers/limits.h ../src/Demand.h
ArrayListJOR.o: ../src/Operation.h ../src/StringVec.h
Att.o: ../src/Att.h ../src/JavaAccObj.h ../src/Includes.h ../src/Macros.h
Att.o: ../src/Forwards.h ../src/Utils.h
Att.o: ../../../config/dummy_headers/stdlib.h
Att.o: ../../../config/dummy_headers/string ../src/AttBldr.h
Att.o: ../src/VectorAtts.h ../../../wit/src/wit.h
Att.o: ../../../config/dummy_headers/stdio.h
Att.o: ../../../config/dummy_headers/limits.h
Att.o: ../../../config/dummy_headers/map ../src/Coordinator.h
Att.o: ../src/Problem.h ../src/Component.h ../src/CTVecSupply.h
Att.o: ../../../config/dummy_headers/list
AttBldr.o: ../src/AttBldr.h ../src/VectorAtts.h ../src/Att.h
AttBldr.o: ../src/JavaAccObj.h ../src/Includes.h ../src/Macros.h
AttBldr.o: ../src/Forwards.h ../src/Utils.h
AttBldr.o: ../../../config/dummy_headers/stdlib.h
AttBldr.o: ../../../config/dummy_headers/string ../../../wit/src/wit.h
AttBldr.o: ../../../config/dummy_headers/stdio.h
AttBldr.o: ../../../config/dummy_headers/limits.h
AttBldr.o: ../../../config/dummy_headers/map ../src/ScalarAtts.h
AttBldr.o: ../src/StringJOR.h ../src/JniClient.h
AttBldr.o: ../../../config/dummy_headers/jni.h
AttBldr.o: ../../../config/dummy_headers/iostream
AttBldrCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
AttBldrCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
AttBldrCOR.o: ../src/Forwards.h ../src/Utils.h
AttBldrCOR.o: ../../../config/dummy_headers/stdlib.h
AttBldrCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
AttBldrCOR.o: ../src/AttBldr.h ../src/VectorAtts.h ../src/Att.h
AttBldrCOR.o: ../src/JavaAccObj.h ../../../config/dummy_headers/string
AttBldrCOR.o: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
AttBldrCOR.o: ../../../config/dummy_headers/limits.h
AttBldrCOR.o: ../../../config/dummy_headers/map ../src/StringJOR.h
AttCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h ../src/JniClient.h
AttCOR.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
AttCOR.o: ../../../config/dummy_headers/stdlib.h
AttCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
AttCOR.o: ../src/Att.h ../src/JavaAccObj.h
AttCOR.o: ../../../config/dummy_headers/string ../src/AttBldr.h
AttCOR.o: ../src/VectorAtts.h ../../../wit/src/wit.h
AttCOR.o: ../../../config/dummy_headers/stdio.h
AttCOR.o: ../../../config/dummy_headers/limits.h
AttCOR.o: ../../../config/dummy_headers/map ../src/Component.h
AttCOR.o: ../src/CTVecSupply.h ../../../config/dummy_headers/list
AttCOR.o: ../src/ArrayJOR.h ../src/StringJOR.h
BomEntry.o: ../src/BomEntry.h ../src/Component.h ../src/JavaAccObj.h
BomEntry.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
BomEntry.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
BomEntry.o: ../src/Part.h ../../../wit/src/wit.h
BomEntry.o: ../../../config/dummy_headers/stdio.h
BomEntry.o: ../../../config/dummy_headers/limits.h
BomEntry.o: ../../../config/dummy_headers/string ../src/Operation.h
BomEntry.o: ../src/Coordinator.h ../../../config/dummy_headers/map
BomEntryCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
BomEntryCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
BomEntryCOR.o: ../src/Forwards.h ../src/Utils.h
BomEntryCOR.o: ../../../config/dummy_headers/stdlib.h
BomEntryCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
BomEntryCOR.o: ../src/BomEntry.h ../src/Component.h ../src/JavaAccObj.h
BopEntry.o: ../src/BopEntry.h ../src/Component.h ../src/JavaAccObj.h
BopEntry.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
BopEntry.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
BopEntry.o: ../src/Part.h ../../../wit/src/wit.h
BopEntry.o: ../../../config/dummy_headers/stdio.h
BopEntry.o: ../../../config/dummy_headers/limits.h
BopEntry.o: ../../../config/dummy_headers/string ../src/Operation.h
BopEntry.o: ../src/Coordinator.h ../../../config/dummy_headers/map
BopEntryCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
BopEntryCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
BopEntryCOR.o: ../src/Forwards.h ../src/Utils.h
BopEntryCOR.o: ../../../config/dummy_headers/stdlib.h
BopEntryCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
BopEntryCOR.o: ../src/BopEntry.h ../src/Component.h ../src/JavaAccObj.h
CTVecSupply.o: ../src/CTVecSupply.h ../src/Includes.h ../src/Macros.h
CTVecSupply.o: ../src/Forwards.h ../src/Utils.h
CTVecSupply.o: ../../../config/dummy_headers/stdlib.h
CTVecSupply.o: ../../../config/dummy_headers/list
CaseToken.o: ../src/CaseToken.h ../src/JavaAccObj.h ../src/Includes.h
CaseToken.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
CaseToken.o: ../../../config/dummy_headers/stdlib.h ../../../wit/src/wit.h
CaseToken.o: ../../../config/dummy_headers/stdio.h
CaseToken.o: ../../../config/dummy_headers/limits.h ../src/Coordinator.h
CaseToken.o: ../../../config/dummy_headers/string
CaseToken.o: ../../../config/dummy_headers/map
CaseTokenCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
CaseTokenCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
CaseTokenCOR.o: ../src/Forwards.h ../src/Utils.h
CaseTokenCOR.o: ../../../config/dummy_headers/stdlib.h
CaseTokenCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
CaseTokenCOR.o: ../src/CaseToken.h ../src/JavaAccObj.h ../../../wit/src/wit.h
CaseTokenCOR.o: ../../../config/dummy_headers/stdio.h
CaseTokenCOR.o: ../../../config/dummy_headers/limits.h
Component.o: ../src/Component.h ../src/JavaAccObj.h ../src/Includes.h
Component.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Component.o: ../../../config/dummy_headers/stdlib.h ../src/ComponentJOR.h
Component.o: ../src/JniClient.h ../../../config/dummy_headers/jni.h
Component.o: ../src/Coordinator.h ../../../config/dummy_headers/string
Component.o: ../../../config/dummy_headers/map ../src/Problem.h
Component.o: ../src/CaseToken.h ../../../wit/src/wit.h
Component.o: ../../../config/dummy_headers/stdio.h
Component.o: ../../../config/dummy_headers/limits.h ../src/CTVecSupply.h
Component.o: ../../../config/dummy_headers/list
ComponentCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
ComponentCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
ComponentCOR.o: ../src/Forwards.h ../src/Utils.h
ComponentCOR.o: ../../../config/dummy_headers/stdlib.h
ComponentCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
ComponentCOR.o: ../src/Component.h ../src/JavaAccObj.h ../src/ComponentJOR.h
ComponentCOR.o: ../src/CTVecSupply.h ../../../config/dummy_headers/list
ComponentCOR.o: ../src/ArrayJOR.h ../src/ArrayListJOR.h
ComponentCOR.o: ../../../config/dummy_headers/vector
ComponentJOR.o: ../src/ComponentJOR.h ../src/JniClient.h ../src/Includes.h
ComponentJOR.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
ComponentJOR.o: ../../../config/dummy_headers/stdlib.h
ComponentJOR.o: ../../../config/dummy_headers/jni.h ../src/Problem.h
ComponentJOR.o: ../src/Component.h ../src/JavaAccObj.h ../src/Part.h
ComponentJOR.o: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
ComponentJOR.o: ../../../config/dummy_headers/limits.h
ComponentJOR.o: ../../../config/dummy_headers/string ../src/Operation.h
ComponentJOR.o: ../src/BomEntry.h ../../../config/dummy_headers/iostream
Coordinator.o: ../src/Coordinator.h ../src/JavaAccObj.h ../src/Includes.h
Coordinator.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Coordinator.o: ../../../config/dummy_headers/stdlib.h
Coordinator.o: ../../../config/dummy_headers/string
Coordinator.o: ../../../config/dummy_headers/map ../src/Problem.h
Coordinator.o: ../src/Component.h ../src/CaseToken.h ../../../wit/src/wit.h
Coordinator.o: ../../../config/dummy_headers/stdio.h
Coordinator.o: ../../../config/dummy_headers/limits.h ../src/Part.h
Coordinator.o: ../src/Demand.h ../src/Operation.h ../src/BomEntry.h
Coordinator.o: ../src/Substitute.h ../src/BopEntry.h ../src/PggHandler.h
Coordinator.o: ../src/CTVecSupply.h ../../../config/dummy_headers/list
Coordinator.o: ../src/ComponentJOR.h ../src/JniClient.h
Coordinator.o: ../../../config/dummy_headers/jni.h ../src/ArrayListJOR.h
Coordinator.o: ../../../config/dummy_headers/vector ../src/PreJavaException.h
CoordinatorCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
CoordinatorCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
CoordinatorCOR.o: ../src/Forwards.h ../src/Utils.h
CoordinatorCOR.o: ../../../config/dummy_headers/stdlib.h
CoordinatorCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
CoordinatorCOR.o: ../src/Coordinator.h ../src/JavaAccObj.h
CoordinatorCOR.o: ../../../config/dummy_headers/string
CoordinatorCOR.o: ../../../config/dummy_headers/map
Demand.o: ../src/Demand.h ../src/Component.h ../src/JavaAccObj.h
Demand.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Demand.o: ../../../config/dummy_headers/stdlib.h
Demand.o: ../../../config/dummy_headers/string ../src/Part.h
Demand.o: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
Demand.o: ../../../config/dummy_headers/limits.h ../src/Operation.h
Demand.o: ../src/Substitute.h ../src/BopEntry.h ../src/Coordinator.h
Demand.o: ../../../config/dummy_headers/map ../src/CaseToken.h
Demand.o: ../src/PggHandler.h ../src/ArrayJOR.h ../src/JniClient.h
Demand.o: ../../../config/dummy_headers/jni.h ../src/ArrayListJOR.h
Demand.o: ../../../config/dummy_headers/vector
DemandCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
DemandCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
DemandCOR.o: ../src/Forwards.h ../src/Utils.h
DemandCOR.o: ../../../config/dummy_headers/stdlib.h
DemandCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
DemandCOR.o: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
DemandCOR.o: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
DemandCOR.o: ../../../config/dummy_headers/limits.h
DemandCOR.o: ../../../config/dummy_headers/string ../src/Demand.h
ExceptionHandler.o: ../src/ExceptionHandler.h ../src/JniClient.h
ExceptionHandler.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
ExceptionHandler.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
ExceptionHandler.o: ../../../config/dummy_headers/jni.h
ExceptionHandler.o: ../src/PreJavaException.h
ExceptionHandler.o: ../../../config/dummy_headers/string
ExceptionHandler.o: ../../../wit/src/wit.h
ExceptionHandler.o: ../../../config/dummy_headers/stdio.h
ExceptionHandler.o: ../../../config/dummy_headers/limits.h
JavaAccObj.o: ../src/Coordinator.h ../src/JavaAccObj.h ../src/Includes.h
JavaAccObj.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
JavaAccObj.o: ../../../config/dummy_headers/stdlib.h
JavaAccObj.o: ../../../config/dummy_headers/string
JavaAccObj.o: ../../../config/dummy_headers/map
JavaAccObjCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
JavaAccObjCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
JavaAccObjCOR.o: ../src/Forwards.h ../src/Utils.h
JavaAccObjCOR.o: ../../../config/dummy_headers/stdlib.h
JavaAccObjCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
JavaAccObjCOR.o: ../src/ComponentJOR.h ../src/JavaAccObj.h
JavaAccObjCOR.o: ../src/ArrayListJOR.h ../../../config/dummy_headers/vector
JniClient.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
JniClient.o: ../src/Forwards.h ../src/Utils.h
JniClient.o: ../../../config/dummy_headers/stdlib.h
JniClient.o: ../../../config/dummy_headers/jni.h ../src/JavaAccObj.h
JniHandler.o: ../src/JniHandler.h ../src/JniClient.h ../src/Includes.h
JniHandler.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
JniHandler.o: ../../../config/dummy_headers/stdlib.h
JniHandler.o: ../../../config/dummy_headers/jni.h
MessageMgr.o: ../src/MessageMgr.h ../src/JavaAccObj.h ../src/Includes.h
MessageMgr.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
MessageMgr.o: ../../../config/dummy_headers/stdlib.h ../src/StringJOR.h
MessageMgr.o: ../src/JniClient.h ../../../config/dummy_headers/jni.h
MessageMgr.o: ../src/CaseToken.h ../../../wit/src/wit.h
MessageMgr.o: ../../../config/dummy_headers/stdio.h
MessageMgr.o: ../../../config/dummy_headers/limits.h
MessageMgr.o: ../../../config/dummy_headers/string
MessageMgrCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
MessageMgrCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
MessageMgrCOR.o: ../src/Forwards.h ../src/Utils.h
MessageMgrCOR.o: ../../../config/dummy_headers/stdlib.h
MessageMgrCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
MessageMgrCOR.o: ../src/MessageMgr.h ../src/JavaAccObj.h ../src/StringJOR.h
Operation.o: ../src/Operation.h ../src/Component.h ../src/JavaAccObj.h
Operation.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Operation.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Operation.o: ../../../config/dummy_headers/string ../src/Coordinator.h
Operation.o: ../../../config/dummy_headers/map ../src/PggHandler.h
Operation.o: ../src/CaseToken.h ../../../wit/src/wit.h
Operation.o: ../../../config/dummy_headers/stdio.h
Operation.o: ../../../config/dummy_headers/limits.h ../src/ArrayJOR.h
Operation.o: ../src/JniClient.h ../../../config/dummy_headers/jni.h
OperationCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
OperationCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
OperationCOR.o: ../src/Forwards.h ../src/Utils.h
OperationCOR.o: ../../../config/dummy_headers/stdlib.h
OperationCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
OperationCOR.o: ../src/Operation.h ../src/Component.h ../src/JavaAccObj.h
OperationCOR.o: ../../../config/dummy_headers/string
Part.o: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
Part.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Part.o: ../../../config/dummy_headers/stdlib.h ../../../wit/src/wit.h
Part.o: ../../../config/dummy_headers/stdio.h
Part.o: ../../../config/dummy_headers/limits.h
Part.o: ../../../config/dummy_headers/string ../src/Coordinator.h
Part.o: ../../../config/dummy_headers/map ../src/CaseToken.h
Part.o: ../src/ArrayJOR.h ../src/JniClient.h
Part.o: ../../../config/dummy_headers/jni.h
PartCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h ../src/JniClient.h
PartCOR.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
PartCOR.o: ../../../config/dummy_headers/stdlib.h
PartCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
PartCOR.o: ../src/Part.h ../src/Component.h ../src/JavaAccObj.h
PartCOR.o: ../../../wit/src/wit.h ../../../config/dummy_headers/stdio.h
PartCOR.o: ../../../config/dummy_headers/limits.h
PartCOR.o: ../../../config/dummy_headers/string ../src/Coordinator.h
PartCOR.o: ../../../config/dummy_headers/map ../src/ArrayListJOR.h
PartCOR.o: ../../../config/dummy_headers/vector
PggHandler.o: ../src/PggHandler.h ../src/Includes.h ../src/Macros.h
PggHandler.o: ../src/Forwards.h ../src/Utils.h
PggHandler.o: ../../../config/dummy_headers/stdlib.h ../src/Coordinator.h
PggHandler.o: ../src/JavaAccObj.h ../../../config/dummy_headers/string
PggHandler.o: ../../../config/dummy_headers/map ../src/Part.h
PggHandler.o: ../src/Component.h ../../../wit/src/wit.h
PggHandler.o: ../../../config/dummy_headers/stdio.h
PggHandler.o: ../../../config/dummy_headers/limits.h ../src/Operation.h
PggHandler.o: ../src/Substitute.h ../src/BopEntry.h ../src/Demand.h
PggHandler.o: ../src/ArrayListJOR.h ../src/JniClient.h
PggHandler.o: ../../../config/dummy_headers/jni.h
PggHandler.o: ../../../config/dummy_headers/vector
PreJavaException.o: ../src/PreJavaException.h ../src/Includes.h
PreJavaException.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
PreJavaException.o: ../../../config/dummy_headers/stdlib.h
PreJavaException.o: ../../../config/dummy_headers/string
Problem.o: ../src/Problem.h ../src/Component.h ../src/JavaAccObj.h
Problem.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h ../src/Utils.h
Problem.o: ../../../config/dummy_headers/stdlib.h ../src/Coordinator.h
Problem.o: ../../../config/dummy_headers/string
Problem.o: ../../../config/dummy_headers/map ../src/Demand.h
Problem.o: ../src/CaseToken.h ../../../wit/src/wit.h
Problem.o: ../../../config/dummy_headers/stdio.h
Problem.o: ../../../config/dummy_headers/limits.h ../src/StringVec.h
Problem.o: ../../../config/dummy_headers/vector ../src/PreJavaException.h
Problem.o: ../src/ComponentJOR.h ../src/JniClient.h
Problem.o: ../../../config/dummy_headers/jni.h ../src/StringJOR.h
Problem.o: ../src/ArrayListJOR.h ../../../config/dummy_headers/iostream
Problem.o: ../../../config/dummy_headers/string.h
ProblemCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
ProblemCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
ProblemCOR.o: ../src/Forwards.h ../src/Utils.h
ProblemCOR.o: ../../../config/dummy_headers/stdlib.h
ProblemCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
ProblemCOR.o: ../src/Problem.h ../src/Component.h ../src/JavaAccObj.h
ProblemCOR.o: ../src/Coordinator.h ../../../config/dummy_headers/string
ProblemCOR.o: ../../../config/dummy_headers/map ../src/ArrayListJOR.h
ProblemCOR.o: ../../../config/dummy_headers/vector ../src/StringJOR.h
ScalarAtts.o: ../src/ScalarAtts.h ../src/Att.h ../src/JavaAccObj.h
ScalarAtts.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
ScalarAtts.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
ScalarAtts.o: ../../../config/dummy_headers/string ../../../wit/src/wit.h
ScalarAtts.o: ../../../config/dummy_headers/stdio.h
ScalarAtts.o: ../../../config/dummy_headers/limits.h ../src/Part.h
ScalarAtts.o: ../src/Component.h ../src/Demand.h ../src/Operation.h
ScalarAtts.o: ../src/BomEntry.h ../src/Substitute.h ../src/BopEntry.h
ScalarAtts.o: ../src/StringJOR.h ../src/JniClient.h
ScalarAtts.o: ../../../config/dummy_headers/jni.h
StringJOR.o: ../src/StringJOR.h ../src/JniClient.h ../src/Includes.h
StringJOR.o: ../src/Macros.h ../src/Forwards.h ../src/Utils.h
StringJOR.o: ../../../config/dummy_headers/stdlib.h
StringJOR.o: ../../../config/dummy_headers/jni.h
StringVec.o: ../src/StringVec.h ../src/Includes.h ../src/Macros.h
StringVec.o: ../src/Forwards.h ../src/Utils.h
StringVec.o: ../../../config/dummy_headers/stdlib.h
StringVec.o: ../../../config/dummy_headers/vector
Substitute.o: ../src/Substitute.h ../src/Component.h ../src/JavaAccObj.h
Substitute.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
Substitute.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
Substitute.o: ../src/BomEntry.h ../src/Part.h ../../../wit/src/wit.h
Substitute.o: ../../../config/dummy_headers/stdio.h
Substitute.o: ../../../config/dummy_headers/limits.h
Substitute.o: ../../../config/dummy_headers/string ../src/Coordinator.h
Substitute.o: ../../../config/dummy_headers/map
SubstituteCOR.o: ../src/NativeMacros.h ../src/ExceptionHandler.h
SubstituteCOR.o: ../src/JniClient.h ../src/Includes.h ../src/Macros.h
SubstituteCOR.o: ../src/Forwards.h ../src/Utils.h
SubstituteCOR.o: ../../../config/dummy_headers/stdlib.h
SubstituteCOR.o: ../../../config/dummy_headers/jni.h ../src/JniHandler.h
SubstituteCOR.o: ../src/Substitute.h ../src/Component.h ../src/JavaAccObj.h
Utils.o: ../src/PreJavaException.h ../src/Includes.h ../src/Macros.h
Utils.o: ../src/Forwards.h ../src/Utils.h
Utils.o: ../../../config/dummy_headers/stdlib.h
Utils.o: ../../../config/dummy_headers/string ../../../wit/src/wit.h
Utils.o: ../../../config/dummy_headers/stdio.h
Utils.o: ../../../config/dummy_headers/limits.h
Utils.o: ../../../config/dummy_headers/iostream
Utils.o: ../../../config/dummy_headers/string.h
VectorAtts.o: ../src/VectorAtts.h ../src/Att.h ../src/JavaAccObj.h
VectorAtts.o: ../src/Includes.h ../src/Macros.h ../src/Forwards.h
VectorAtts.o: ../src/Utils.h ../../../config/dummy_headers/stdlib.h
VectorAtts.o: ../../../config/dummy_headers/string ../../../wit/src/wit.h
VectorAtts.o: ../../../config/dummy_headers/stdio.h
VectorAtts.o: ../../../config/dummy_headers/limits.h ../src/Part.h
VectorAtts.o: ../src/Component.h ../src/Demand.h ../src/Operation.h
VectorAtts.o: ../src/BomEntry.h ../src/Substitute.h ../src/BopEntry.h
VectorAtts.o: ../src/ArrayJOR.h ../src/JniClient.h
VectorAtts.o: ../../../config/dummy_headers/jni.h

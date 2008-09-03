"""
Generate idl from function definition
"""

import pytypes

# return string: function definition, looks like this:
# type FunctionName([name:type]*);
def genIdlFn(func):
    idlFnString = pytypes.genTypeName(func.return_type)
    idlFnString += ' ' + func.name
    
    argString = ''
    for arg in func.arguments:
        argString += pytypes.genTypeName(arg.type)
        argString += ' ' + arg.name + ', '
        
    argString = argString.rstrip(', ')
    
    return idlFnString + '(' + argString + ');'
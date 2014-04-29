import sys
import glob
import re
from timing_file_parser import parseTimingFile
from maps import *

STRUCTURE_TEMPLATE = r"""\multirow{$NUM_OPERATIONS$}{*}{$STRUCTURE$} & $OPERATION$ & $X_VALUES$ \\\\"""

TABLE_TEMPLATE = r"""
\begin{table}
	\centering
	\makebox[\textwidth][c]{%
		\begin{tabular}{|r|r|$X_COLUMNS$|}
			\hline
			\multicolumn{2}{|c}{} & \multicolumn{$NUM_X$}{|c|}{\textbf{$X_AXIS$}} \\
			\hline
			\textbf{Structure} & \textbf{Operation} & $X_VALUES$ \\
			\hline
			$STRUCTURES$
		\end{tabular}
	}%

	\caption{$CAPTION$}
	\label{TODO}
\end{table}
"""

def replaceValue(string, varToReplace, replacement):
	pattern = re.compile(r"\$" + varToReplace + r"\$")
	return pattern.sub(replacement, string)

def replaceUnderlines(string):
	return string.replace("_", "\\_")

def extractValues(data, operation, structure):
	values = []
	for x, y in data[operation][structure]:
		if y < 0:
			values.append("-")
		else:
			values.append(y)
	return values

def roundValue(value, precision):
	if value == "-":
		return value
	else:
		formatStr = "%06." + str(precision) + "f"
		return formatStr % float(value)

def generateLatexTable(timingData, fpPrecision):
	tableStr = TABLE_TEMPLATE
	tableStr = replaceValue(tableStr, "X_AXIS", timingData.xAxis)
	# Determine all x values in total (should already be sorted)
	xValues = []
	for operation in timingData.data:
		for structure in timingData.data[operation]:
			xValues = [ x[0] for x in timingData.data[operation][structure] ]
			break
		if xValues != -1:
			break
	if len(xValues) == 0:
		return "ERROR: Could not determine X values!"

	tableStr = replaceValue(tableStr, "NUM_X", str(len(xValues)))
	xValuesStr = " & ".join( [ str(x) for x in xValues ] )
	tableStr = replaceValue(tableStr, "X_VALUES", xValuesStr)
	xColumnsStr = "|".join( [ "l" for x in xValues ] )
	tableStr = replaceValue(tableStr, "X_COLUMNS", xColumnsStr)
	# Make dataset name the caption
	tableStr = replaceValue(tableStr, "CAPTION", data.title)

	# Create template for each structure and combine in string separated by newlines
	# Construct list of all structures and SORT IT APLHABETICALLY
	structureNames = []
	for operation in timingData.data:
		structureNames = sorted(timingData.data[operation].keys())
		break
	operationNames = sorted(timingData.data.keys())
	# For each structure, construct entry
	structureEntries = []
	for struct in structureNames:
		structStr = STRUCTURE_TEMPLATE
		# Fill in first line of structure string
		structStr = replaceValue(structStr, "NUM_OPERATIONS", str(len(timingData.data)))
		structStr = replaceValue(structStr, "STRUCTURE", STRUCTURE_NAME_MAP[struct])
		structStr = replaceValue(structStr, "OPERATION", OPERATION_NAME_MAP[operationNames[0]])
		# Construct list containing all actual values (Y))
		values = extractValues(timingData.data, operationNames[0], struct)
		valueStr =  " & ".join([ roundValue(x, fpPrecision) for x in values ])
		structStr = replaceValue(structStr, "X_VALUES", valueStr)
		structStr += "\n"
		# Add line for each other operation
		for i in range(1, len(operationNames)):
			opName = operationNames[i]
			values = values = extractValues(timingData.data, opName, struct)
			valueStr = " & ".join([ roundValue(x, fpPrecision)  for x in values ])
			structStr += "& %s & %s \\\\\\\\ \n" % (OPERATION_NAME_MAP[opName], valueStr)
		# Finish with a hline
		structStr += "\\hline"

		structureEntries.append(structStr)

	allStructureEntries = "\n".join(structureEntries)
	tableStr = replaceValue(tableStr, "STRUCTURES", allStructureEntries)

	return tableStr

if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\" {<floatingPointPrecision>}" % sys.argv[0])
	timingFilenameGlob = sys.argv[1]
	if len(sys.argv) >= 3:
		floatingPointPrecision = int(sys.argv[2])
	else:
		floatingPointPrecision = 8

	# Use glob to find all desired files
	filenames = glob.glob(timingFilenameGlob)
	# Process each timing file in turn
	for fname in filenames:
		# Retrieve contents of timing file
		data = parseTimingFile(fname, False)
		# Generate and output LaTeX table
		tableStr = generateLatexTable(data, floatingPointPrecision)
		print(tableStr)
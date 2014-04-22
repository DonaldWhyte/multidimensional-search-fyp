import sys
import glob
import re
from timing_file_parser import parseTimingFile

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

	\caption{TODO}
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

def generateLatexTable(timingData):
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

	# Create template for each structure and combine in string separated by newlines
	# Construct list of all structures and SORT IT APLHABETICALLY
	structureNames = []
	for operation in timingData.data:
		structureNames = sorted(timingData.data[operation].keys())
		break
	print structureNames
	operationNames = sorted(timingData.data.keys())
	# For each structure, construct entry
	structureEntries = []
	for struct in structureNames:
		structStr = STRUCTURE_TEMPLATE
		# Fill in first line of structure string
		structStr = replaceValue(structStr, "NUM_OPERATIONS", str(len(timingData.data)))
		structStr = replaceValue(structStr, "STRUCTURE", replaceUnderlines(struct))
		structStr = replaceValue(structStr, "OPERATION", operationNames[0])
		# Construct list containing all actual values (Y))
		values = extractValues(timingData.data, operationNames[0], struct)
		valueStr =  "& ".join([ str(x) for x in values ])
		structStr = replaceValue(structStr, "X_VALUES", valueStr)
		structStr += "\n"
		# Add line for each other operation
		for i in range(1, len(operationNames)):
			opName = operationNames[i]
			values = values = extractValues(timingData.data, opName, struct)
			valueStr = " & ".join([ str(x) for x in values ])
			structStr += "& {} & {} \\\\\\\\ \n".format(opName, valueStr)
		# Finish with a hline
		structStr += "\\hline"

		structureEntries.append(structStr)

	allStructureEntries = "\n".join(structureEntries)
	tableStr = replaceValue(tableStr, "STRUCTURES", allStructureEntries)

	return tableStr

if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\"" % sys.argv[0])
	timingFilenameGlob = sys.argv[1]
	# Use glob to find all desired files
	filenames = glob.glob(timingFilenameGlob)
	# Process each timing file in turn
	for fname in filenames:
		# Retrieve contents of timing file
		data = parseTimingFile(fname)
		# TODO
		tableStr = generateLatexTable(data)
		print(tableStr)
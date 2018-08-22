# -*- coding: utf-8 -*-
import sys
from xml.etree.ElementTree import parse, dump

#  increase the version.
version_xml = parse('Version.xml')
root = version_xml.getroot()
version = int(root.text) + 1
root.text = str(version)
version_xml.write('Version.xml')
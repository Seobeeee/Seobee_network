# -*- coding: utf-8 -*-
from optparse import OptionParser
import sys
from xml.etree.ElementTree import parse, dump
from mako.template import Template

supported_types = ["CInt8", "Int16", "Int32", "UInt8", "UInt16", "UInt32", "Int64", "UInt64", "Float", "Double", "Bool",
                   "String"]


enum_list = []

class Member(object):
    def __init__(self, class_name, xml):
        self.type = xml.get("type")
        self.value = xml.get("value")
        self.name = xml.get("name")

    def render(self):
        pass


class Packet(object):
    def __init__(self, xml, class_template, is_enum=False):
        self.name = xml.get("name")
        self.id = abs(hash(self.name) & 0xFFFFFFFF)
        self.member_list = list()
        self.template = Template(filename=class_template)
        params = xml.findall("param")

        if is_enum:
            enum_list.append(self.name)

        for param in params:
            member = Member(self.name, param)
            member.is_enum =  not is_enum and member.type in enum_list

            self.member_list.append(member)


    def render(self, packet_name):
        return self.template.render(class_name=packet_name, class_data=self)


def render(xml_file, options, out_file_name,  file_name, version):
    xml = parse(xml_file)
    protocol = xml.getroot()

    template = Template(filename=options.protocol_template)

    packet_list = list()
    packets = protocol.findall("packet")
    enums = protocol.findall("enum")
    structs = protocol.findall("struct")

    for packet in enums:
        packet_list.append(Packet(packet, options.enum_template, True))
    for packet in structs:
        packet_list.append(Packet(packet, options.struct_template))
    for packet in packets:
        packet_list.append(Packet(packet, options.class_template))

    data = template.render(packet_list=packet_list, file_name=file_name, version=version)
    data = data.encode('utf8')
    data = data.replace('\r\n', '\n')
    with open(out_file_name, "w") as f:
        f.write(data)

def main():
    parser = OptionParser()
    parser.add_option("-p", "--protocol_template", dest="protocol_template", default="protocol_template.h")
    parser.add_option("-t", "--class_template", dest="class_template", default="class_template.h")
    parser.add_option("-e", "--enum_template", dest="enum_template", default="enum_template.h")
    parser.add_option("-s", "--struct_template", dest="struct_template", default="struct_template.h")
    parser.add_option("-o", "--output", dest="out_dir", default="./", help="output directory")
    parser.add_option("-c", "--cpp_out", action="store_true", dest="is_cpp", default=False)
    parser.add_option("-H", "--cpp_h_out", action="store_true", dest="is_h", default=False)
    parser.add_option("-C", "--cs_out", action="store_true", dest="is_cs", default=False)

    (options, args) = parser.parse_args()

    xml_file = args[0]
    file_name = xml_file[0:xml_file.find(".xml")]
    out_file_name = file_name

    if options.is_cpp:
        out_file_name += ".cpp"
    elif options.is_h:
        out_file_name += ".h"
    elif options.is_cs:
        out_file_name += ".cs"
    else:
        return

    #  increase the version.

    version_xml = parse('Version.xml')
    root = version_xml.getroot()
    version = int(root.text)

    out_file_name = options.out_dir + out_file_name
    render(xml_file, options, out_file_name, file_name, version)

if __name__ == "__main__":
    main()

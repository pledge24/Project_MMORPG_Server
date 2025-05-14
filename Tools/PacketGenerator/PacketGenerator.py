import argparse
import jinja2
import ProtoParser
from pathlib import Path

def main():

	# CLI 인자를 담당하는 argparse 객체를 하나 만든다.
	# arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')

	# pathlib 사용 (추천: Python 3.4+)
	current_file = Path(__file__)
	base_dir = current_file.parents[2]
	target_file = base_dir/'Common'/'protoc-21.12-win64'/'bin'/'Protocol.proto'
	print(target_file.resolve()) # 상대 경로 -> 절대 경로

	# # argparse 객체에 CLI 명령줄에서 사용가능한 인자 종류를 추가한다.
	# arg_parser.add_argument('--path', type=str, default='C:\Rookiss/Mentoring/UE5_Mentoring_Temp/MMO/Server/Common/protoc-21.12-win64/bin/Protocol.proto', help='proto path')
	# arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
	# arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	# arg_parser.add_argument('--send', type=str, default='S_', help='send convention')

	# # 명령줄(ex. python PacketGenerator.py --path "C:/Game.proto")에서 제공된 인자를 파싱하여,
	# # args 객체에 저장
	# args = arg_parser.parse_args()

	# # ProtoParser 객체를 하나 생성한 다음, 인자를 토대로 Protocol.proto의 패킷 이름을 추출한다.
	# parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	# parser.parse_proto(args.path)

	# # 템플릿 파일이 들어있는 디렉토리 위치 지정. 디렉토리 이름은 "Templates" 
	# file_loader = jinja2.FileSystemLoader('Templates')

	# # jinja2 환경 객체를 생성. loader 옵션은 위에서 만든 file_loader로 지정한다.
	# env = jinja2.Environment(loader=file_loader)

	# # Templates에 들어있는 템플릿 코드 'PacketHandler.h'를 들고 온다.
	# template = env.get_template('PacketHandler.h')

	# # 가져온 템플릿에 parse와 output을 넘겨 렌더링하고 최종 출력물을 얻는다.
	# output = template.render(parser=parser, output=args.output)

	# # 얻어낸 최종 출력물을 원하는 코드에 붙여넣는다(없다면 생성한다.)
	# f = open(args.output+'.h', 'w+')
	# f.write(output)
	# f.close()

	# print(output)
	return

if __name__ == '__main__':
	main()
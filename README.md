ImageDomino
===========

이미지 이어 붙이기 프로젝트(Qt 사용)

Qt를 사용한 이미지 이어붙이기 프로젝트

개발배경
인터넷을 돌아다니다 보면 방송캡처를 이어 붙인 이미지를 보게 되는데, 예전에 나도 좋은 내용을 알리고 싶어 일일히
동영상을 보면서 캡처하고 자막을 넣고 그랬던 적이 있다. 그 때는 포토샵과 비슷한 국내 툴을 이용했는데 하나하나 파일을 열어서
복사 + 붙여넣고 자막을 넣고 하는 작업이 정말 시간이 오래걸렸다.
그래서 간단하게 이미지를 이어 붙이고 싶은 마음에 프로젝트를 시작했다.


같은 프로그램이 없나 개발중 조사 하다보니...
ImageMagick이라는 훌륭한 프로그램이 있었다.
Photoscape라는 툴에서도 이미지를 이어붙이는 것이 가능했었다.
원래 구상한 UI가 있었는데 Photoscape쪽 UI가 마음에 들어서 비슷하게 구현해보았다.


왜 Qt를 선택?
임베디드 보드에서 Qt를 이용해서 서버, 클라이언트 프로그램을 잠깐 짜봤는데 재미있는 부분을 가진 프레임워크라 생각해서
Qt를 조금 더 알아가기 위해 만들어 보고 싶었다.


프로젝트를 진행하면서..
솔직히 말해서 Qt는 자료가 MFC나 C#보다 적다. 문서가 있다고 하지만 영어에 익숙하지 않은 이상 한글자료가 좋지 않은가..?
그래도 재미있는건 사실이다.


Qt 프레임워크의 버그
내가 발견했다고 쓰는것도 우습지만 무언가 QFileSystemModel 에서의 버그가 존재하는 것같다.
QFileSystemModel 에서 File만 보이도록 하고, 디렉토리를 자꾸 바꿔보면 파일만 나와야 할 Model에 디렉토리까지 같이 보일때가 있다.
예상되는건 QFileSystemModel의 Cache문제인듯한데.. 찾아봐도 잘 모르겠더라..
구글에 찾아보면 내가 말한것과 같은 문제를 겪는사람이 있는데.. 그 사람도 결국 답은 못얻었었다.


버그?
프로젝트에 버그는 있을 것이다.
요즘 취업을 준비하고 있는 중이라 빠른 수정을 하지는 못할 듯하다.





Todo
===========
 - 이미지 해상도 관련 처리


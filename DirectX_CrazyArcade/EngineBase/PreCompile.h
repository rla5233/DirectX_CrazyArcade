#pragma once

#include <WS2tcpip.h> //������
#include <WinSock2.h> //������
#include <Windows.h>
#include <assert.h>
#include <string>
#include <string_view>
#include <functional>
#include <memory>

#include <map>
#include <list>
#include <set>
#include <vector>

#include <chrono>

#include <thread>
#include <mutex>
#include <atomic>



// ���̷�Ʈ�� �������� �κ�
#include <DirectXPackedVector.h> // ���̷�Ʈ���� ���
#include <DirectXCollision.h> 
// ���̷�Ʈ�� �浹��� <= ���� �浹���� ���մϴ�. <= ���̷�Ʈ�� �������ִµ� ���� �ָ����
// ������ �������� �����ְڽ��ϴ�.
// �������°� 2~3������ �̷��� ���� ������. 
// �׷���ī������ ���� �����ϰ� cpu���� ����ȭ�� �Ǿ� �ִ�.
// SIMD �����̶���� <= ���� ����

/*
 * EventLoop.cpp
 *
 * Created: 26.05.2016 14:46:10
 *  Author: user
 */

#include "graphobj.hpp"
#include "../PlatformDrv.hpp"

 //������, �� �������� ���������� ����������� ������ ����� �� ��������� ������
const signal NextObjectSignal = signal::BtnDelUp;			//���������� ������ Del

void PushSignal(const signal sig);

void GuiEventLoop() {
	layer *CurLayer = layer::layer_stack.pick();
	GraphObj *SelectedObject = CurLayer->head, *found;
	signal sig;

	for (found = SelectedObject;;) {				//���� ������ ������, ������� ������� ����� �����
		if (found->CanReciveFocus) {
			found->SetFocus();
			break;
		}
		found = found->next;
		if (found == nullptr) {
			found = CurLayer->head;
		}
	}

	while (1) {
		switch ((sig = WaitSignal())) {
			default:
				if (SelectedObject != nullptr && SelectedObject->CanReciveFocus && SelectedObject->Enabled) {
					SelectedObject->ProcSignal(sig);
				}
				if (!SelectedObject->Enabled) {		//�� ������, ���� ���������� ������ ���� ���������� ��������� ����� �������
					PushSignal(NextObjectSignal);
				}
				break;

			case NextObjectSignal:
				for (found = SelectedObject;;) {				//���� ��������� ������, ������� ������� ����� �����
					found = found->next;
					if (found == nullptr) {
						found = CurLayer->head;
					}
					if ((found->CanReciveFocus && found->Enabled) || found == SelectedObject) {
						break;
					}
				}
				if (found != SelectedObject) {		//���� ��������� ������ �� ��� �� �����, ��� ��� �������
					SelectedObject->RemoveFocus();
					SelectedObject = found;
					SelectedObject->SetFocus();
				}
				break;

			case signal::BreakGuiLoop:
				if (CurLayer->prev != nullptr) {		//������ ���������� ���� �� ����� ������ ����, �������� ������
					return;
				}
		}
	}
}

void GuiEventLoop_noblock(VoidFuncPtr userHook) {
	layer *CurLayer = layer::layer_stack.pick();
	GraphObj *SelectedObject = CurLayer->head, *found;
	signal sig;

	while (1) {
		switch ((sig = GetSignal())) {			//���������� ������������� ������� ��� ��������� �������
			default:
				if (SelectedObject != nullptr && SelectedObject->CanReciveFocus && SelectedObject->Enabled && sig != signal::none) {
					SelectedObject->ProcSignal(sig);
				}
				break;

			case NextObjectSignal:
				for (found = SelectedObject;;) {
					found = found->next;
					if (found == nullptr) {
						found = CurLayer->head;
					}
					if ((found->CanReciveFocus && found->Enabled) || found == SelectedObject) {
						break;
					}
				}
				if (found != SelectedObject) {
					SelectedObject->RemoveFocus();
					SelectedObject = found;
					SelectedObject->SetFocus();
				}
				break;

			case signal::BreakGuiLoop:
				if (CurLayer->prev != nullptr) {
					return;
				}
			}
			if (userHook != nullptr) {
				userHook();
			}
	}
}

import VERTC from '@byteplus/rtc';
import { useState } from 'react';
import { Popover } from 'antd';
import BytePlusLogo from '@/assets/img/BytePlusLogo.png';
import Setting from '@/assets/img/Setting.svg';
import styles from './index.module.less';
import Icon from '../Icon';
import { DEMO_VERSION, TOS, PrivacyPolicy } from '@/config';

interface HeaderProps {
  children?: React.ReactNode;
  hide?: boolean;
}

function Header(props: HeaderProps) {
  const { children, hide } = props;
  const [popoverOpen, setPopoverOpen] = useState<boolean>(false);

  return (
    <div
      className={styles.header}
      style={{
        display: hide ? 'none' : 'flex',
      }}
    >
      <div className={styles['header-logo']}>
        <div className={styles['header-byteplus']}>
          <img src={BytePlusLogo} alt="Logo" />
          <div className={styles['header-text']}>
            <span className={styles['header-rtc']}>RTC</span>
            <span className={styles['header-demo']}>Demo</span>
          </div>
        </div>
      </div>

      {children}

      <div className={styles['header-right']}>
        <span className={styles['header-right-text']}>Demo Version:{DEMO_VERSION}</span>
        <span className={styles['header-right-text']}> / </span>
        <span className={styles['header-right-text']}>
          <span>SDK Version:{VERTC.getSdkVersion()}</span>
        </span>
        <Popover
          trigger="click"
          title={null}
          visible={popoverOpen}
          overlayClassName={styles['header-pop']}
          onVisibleChange={setPopoverOpen}
          content={
            <ul>
              <li
                onClick={() => {
                  setPopoverOpen(false);
                  window.open(TOS, '_blank');
                }}
              >
                Terms of Service
              </li>
              <li
                onClick={() => {
                  setPopoverOpen(false);
                  window.open(PrivacyPolicy, '_blank');
                }}
              >
                Privacy Policy
              </li>
            </ul>
          }
        >
          <button className={styles['header-setting-btn']}>
            <Icon src={Setting} />
          </button>
        </Popover>
      </div>
    </div>
  );
}

export default Header;
